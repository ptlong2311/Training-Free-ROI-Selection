from tkinter import Tk, Entry, Button, Label, ttk
import os
from label_inner import INNER  
from outer import OUTER  
from label_pallet import PALLET

# Định nghĩa hàm inner
def create_inner(data):
    inn = INNER(
        model=data["model"],
        capacity=data["capacity"],
        batch_no=data["batch_no"],
        rank=data["rank"],
        qty=data["qty"],
        date=data["date"],
        remark=data["remark"],
        tracking=data["tracking"]
    )

    inn.create_pdf()
    os.startfile("pdf\\inner.pdf")

# Định nghĩa hàm outer
def create_outer(data):
    out = OUTER(
        model=data["model"],
        capacity=data["capacity"],
        batch_no=data["batch_no"],
        rank=data["rank"],
        qty=data["qty"],
        date=data["date"],
        remark=data["remark"],
        partnumber=data["partnumber"],
        sapBatch=data["batch_no"] + data["rank"],
        tracking=data["tracking"]
    )

    out.create_pdf()
    os.startfile("pdf\\outer.pdf")

# Định nghĩa hàm pallet
def create_pallet(data):
    plet = PALLET(
        model = data["model"],
        capacity = data["capacity"],
        prod_batch = data["prod_batch"],
        rank = data["rank"],
        qty = data["qty"],
        date = data["date"],
        remark = data["remark"],
        partnumber = data["partnumber"],
        sapBatch = data["prod_batch"]+data["rank"],
        tracking = data["tracking"]
    )
    plet.create_pdf()
    os.startfile("pdf\\pallet.pdf")

def generate_pdf():
    label_choice = label_var.get()

    if label_choice == "Inner":
        create_inner({
            "model": model_entry.get(),
            "capacity": capacity_entry.get(),
            "batch_no": batch_no_entry.get(),
            "rank": rank_entry.get(),
            "date": date_entry.get(),
            "qty": qty_entry.get(),
            "remark": remark_combobox.get(),
            "tracking": tracking_entry.get(),
        })
    elif label_choice == "Outer":
        create_outer({
            "model": model_entry.get(),
            "capacity": capacity_entry.get(),
            "batch_no": batch_no_entry.get(),
            "rank": rank_entry.get(),
            "date": date_entry.get(),
            "qty": qty_entry.get(),
            "remark": remark_combobox.get(),
            "partnumber": partnumber_entry.get(),
            "sapBarcode": sap_barcode_entry.get(),
            "tracking": tracking_entry.get(),
        })
    elif label_choice=="Pallet":
       create_pallet({
            "model"             :model_entry.get(),
            "capacity"          :capacity_entry.get(),
            "prod_batch"        :prod_batch_entry.get(),
            "rank"              :rank_entry.get(),
            "qty"               : qty_entry.get(),
            "remark"            :remark_combobox.get(),
            "date"              :date_entry.get(),
            "partnumber"        :partnumber_entry.get(),
            "sapBarcode"          :sap_barcode_entry.get(),
            "tracking"          : tracking_entry.get(),


       })


root = Tk()
root.title("Tạo PDF")
label_var = ttk.Combobox(root, values=["Inner", "Outer","Pallet"])
label_var.set("Inner")
label_var.pack()

model_label = Label(root, text="Model:")
model_label.pack()
model_entry = Entry(root)
model_entry.pack()

capacity_label = Label(root, text="Capacity:")
capacity_label.pack()
capacity_entry = Entry(root)
capacity_entry.pack()

batch_no_label = Label(root, text="Batch No:")
batch_no_label.pack()
batch_no_entry = Entry(root)
batch_no_entry.pack()

prod_batch_label = Label(root, text="Prod Batch:")  
prod_batch_label.pack()
prod_batch_entry = Entry(root)
prod_batch_entry.pack()

rank_label = Label(root, text="Rank:")
rank_label.pack()
rank_entry = Entry(root)
rank_entry.pack()

qty_label = Label(root, text="Qty:")
qty_label.pack()
qty_entry = Entry(root)
qty_entry.pack()

date_label = Label(root, text="Date:")
date_label.pack()
date_entry = Entry(root)
date_entry.pack()

remark_label = Label(root, text="Remark:")
remark_label.pack()
remark_combobox = ttk.Combobox(root, values=["Soc 30% Made in Vietnam"])
remark_combobox.pack()

partnumber_label = Label(root, text="Part Number:")
partnumber_label.pack()
partnumber_entry = Entry(root)
partnumber_entry.pack()

tracking_label = Label(root, text="Tracking:")
tracking_label.pack()
tracking_entry = Entry(root)
tracking_entry.pack()

sap_barcode_label = Label(root, text="Sap's Barcode:")
sap_barcode_label.pack()
sap_barcode_entry = Entry(root)
sap_barcode_entry.pack()

generate_pdf_button = Button(root, text="Tạo PDF", command=generate_pdf)
generate_pdf_button.pack()

root.mainloop()
