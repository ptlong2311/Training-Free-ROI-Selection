import copy
import torch
import json
import argparse
import os
import logging
from tqdm import tqdm
from pathlib import Path
import json


logging.basicConfig(level=logging.INFO)
torch.manual_seed(114514)


def parse_args():
    def str2bool(v):
        return v.lower() in ("1", "true", "yes", "y")

    parser = argparse.ArgumentParser()
    parser.add_argument("--model_type", required=True)
    parser.add_argument("--model_name_or_path")
    parser.add_argument("--screenspot_imgs", required=True)
    parser.add_argument("--screenspot_test", required=True)
    parser.add_argument("--log_path", required=True)
    parser.add_argument("--use_guide_text", type=str2bool, default=True)
    parser.add_argument("--max_pixels", type=int, default=2116800)
    return parser.parse_args()

def build_model(args):
    if args.model_type != "MAI_UI":
        raise ValueError("Only MAI_UI supported")

    from models.MAI_UI import CustomQwen3_VL_VLLM_Model
    model = CustomQwen3_VL_VLLM_Model()
    model.load_model(
        model_name_or_path=args.model_name_or_path,
        max_pixels=args.max_pixels
    )
    return model

def adapt_sample(raw):
    s = copy.deepcopy(raw)

    if "prompt_to_evaluate" in s:
        s["prompt"] = s["prompt_to_evaluate"]
    elif "instruction" in s:
        s["prompt"] = s["instruction"]
    else:
        raise KeyError("Sample has no instruction field")

    # -------- image filename (ABSOLUTE FIX) --------
    if "img_filename" in s:
        s["img_filename"] = os.path.basename(s["img_filename"])
    elif "image_path" in s:
        s["img_filename"] = os.path.basename(s["image_path"])
    else:
        raise KeyError("Sample has no image path")

    # -------- image size --------
    if "img_size" in s:
        pass
    elif "image_size" in s:
        s["img_size"] = s["image_size"]
    else:
        raise KeyError("Sample has no image size")

    # -------- misc --------
    s.setdefault("id", s["img_filename"])
    s.setdefault("platform", s.get("application", "unknown"))
    s.setdefault("application", s.get("platform", "unknown"))
    s.setdefault("ui_type", s.get("element_type", "icon"))
    s.setdefault("group", s.get("category"))

    return s



def eval_positive(sample, response):
    if response["point"] is None:
        return "wrong_format"

    x1, y1, x2, y2 = sample["bbox"]
    w, h = sample["img_size"]

    x1, x2 = x1 / w, x2 / w
    y1, y2 = y1 / h, y2 / h

    px, py = response["point"]
    return "correct" if x1 <= px <= x2 and y1 <= py <= y2 else "wrong"


from pathlib import Path
import json

def load_dataset(json_path: str):
 
    path = Path(json_path)
    dataset = []
    for jf in sorted(path.glob("*.json")):
        with open(jf, "r") as f:
            dataset.extend(json.load(f))

    subset_size = int(len(dataset) * 0.8) 
    return dataset[:subset_size]

def main(args):
    print(args)
    model = build_model(args)
    print("Load model success")

    # ---- load dataset ----
    all_samples = []
    for fn in os.listdir(args.screenspot_test):
        if not fn.endswith(".json"):
            continue
        with open(os.path.join(args.screenspot_test, fn)) as f:
            data = json.load(f)
        all_samples.extend(data)

    print(f"Loaded {len(all_samples)} raw samples")

    samples = [adapt_sample(s) for s in all_samples]

    results = []
    batch_size = 64

    for i in tqdm(range(0, len(samples), batch_size)):
        batch = samples[i:i + batch_size]

        instructions = [s["prompt"] for s in batch]
        images = [
            os.path.join(args.screenspot_imgs, s["img_filename"])
            for s in batch
        ]

        try:
            responses = model.batch_ground_only_positive(
                instructions=instructions,
                images=images,
                use_guide_text=args.use_guide_text
            )
        except Exception as e:
            print("Batch failed → fallback:", e)
            responses = []
            for s in batch:
                try:
                    responses.append(
                        model.ground_only_positive(
                            instruction=s["prompt"],
                            image=os.path.join(args.screenspot_imgs, s["img_filename"]),
                            use_guide_text=args.use_guide_text
                        )
                    )
                except Exception:
                    responses.append({
                        "point": None,
                        "raw_response": "ERROR"
                    })

        for s, r in zip(batch, responses):
            pred_px = None
            if r["point"] is not None:
                w, h = s["img_size"]
                pred_px = [r["point"][0] * w, r["point"][1] * h]

            results.append({
                "id": s["id"],
                "img_path": os.path.join(args.screenspot_imgs, s["img_filename"]),
                "prompt": s["prompt"],
                "bbox": s["bbox"],
                "pred": pred_px,
                "correctness": eval_positive(s, r),
                "raw_response": r["raw_response"]
            })

    os.makedirs(os.path.dirname(args.log_path), exist_ok=True)
    with open(args.log_path, "w") as f:
        json.dump({"details": results}, f, indent=2)

    logging.info("Evaluation finished cleanly.")


if __name__ == "__main__":
    main(parse_args())
