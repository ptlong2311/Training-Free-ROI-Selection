# Training-Free-ROI-Selection
# Installation 
**1. Qwen3VL environment Setup**

```# Clone the repository
git clone https://github.com/Sejong-VLI/VLI-26-01-Long.git
cd..
conda create -n zoom python=3.10.19 -y
conda activate zoom
pip install -r requirements.txt
```
**2. MAI-UI enviroment Setup**
```
# Install vLLM
pip install vllm==0.11.0

# Start vLLM API server (replace MODEL_PATH with your local model path or HuggingFace model ID)
python -m vllm.entrypoints.openai.api_server \
    --model <huggingface_model_path> \
    --served-model-name MAI-UI-8B \
    --host 0.0.0.0 \
    --port 8000 \
    --tensor-parallel-size 1 \
    --trust-remote-code
#Install Dependencies
pip install -r requirements.txt
```
**3. Data Preparation**

**Screenspot-Pro**  
- Download **ScreenSpot-Pro**: [ScreenSpot-Pro dataset](https://github.com/likaixin2000/ScreenSpot-Pro-GUI-Grounding)

**OSWorld**  
- Download **OSWorld**: [OSWorld dataset](https://github.com/xlang-ai/OSWorld-G.git)

**UI-Vision**  
- Download **UI-Vision**: [UI-Vision dataset](https://huggingface.co/datasets/ServiceNow/ui-vision)  
- Recommended directory layout:
```
/path/to/dataset/
  images/
  annotations/
```
**4. Install CLIP ViT-B/32**
- Download **CLIP ViT-B/32** and **CheckPoint**: [CLIP ViT-B32 Model](https://drive.google.com/drive/folders/1nYWcGwybB_VjE5ULjAoRECJVEHcG_Ewq?usp=sharing)

# Evaluation Scripts

- We provide three main evaluation scripts:
```
# Run experiments for Qwen3VL-8B
./eval_sspro.ipynb
./eval_osworld.ipynb
# Run experiments for MAIUI-8B
