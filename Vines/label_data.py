import tkinter as tk
from tkinter import ttk
import json, requests, os
from label_pallet import PALLET
from label_inner import INNER
from label_outer import OUTER
from vntime import VnTimestamp

#Tạo bảng nhập thông tin label
class LabelData:
    def __init__(self, root, inputData):
        self.pdf_button_frame = tk.Frame(root)
        self.rank       = inputData["rank"]
        self.qty        = inputData["qty"]
        self.tracking   =  inputData["barcode"]
        self.prodBatch  =  inputData["prod_batch"]
        self.batchno    =inputData["batch_no"]
        self.typevar    = inputData["typevar"]
        self.dialogue = tk.Toplevel(self.pdf_button_frame)
        self.dialogue.title("CREATE LABEL DATA")
        self.dialogue.geometry("400x300")
        x = root.winfo_x()
        y = root.winfo_y()
        self.dialogue.geometry("+%d+%d" % (x , y ))

        self.open_data_entry_form()

       


    def open_data_entry_form(self):
       

        self.lbModel = tk.Label(self.dialogue, text="Model:")
        self.lbModel.pack()
        self.tbModel = tk.Entry(self.dialogue)
        self.tbModel.pack()

        self.lbCapacity = tk.Label(self.dialogue, text="Capacity:")
        self.lbCapacity.pack()
        self.tbCapacity = tk.Entry(self.dialogue)
        self.tbCapacity.insert(0, "4800mAh")
        self.tbCapacity.pack()

        self.lbRemark = tk.Label(self.dialogue, text="Remark:")
        self.lbRemark.pack()
        self.tbRemark = tk.Entry(self.dialogue)
        self.tbRemark.insert(0, "Soc 30% Made in Vietnam")
        self.tbRemark.pack()

        self.lbDate = tk.Label(self.dialogue, text="Date:")
        self.lbDate.pack()
        self.tbDate = tk.Entry(self.dialogue)
        # self.tbDate.insert(0, VnTimestamp.today_date_str())
        self.tbDate.pack()
        

        self.generate_pdf_button = tk.Button(self.dialogue, text="CREATE LABEL", command=self.create_pdf)
        self.generate_pdf_button.pack()


    def create_pdf(self):
        #data
        outData = {
            "model"         : self.tbModel.get()   ,
            "capacity"      : self.tbCapacity.get()   ,
            "prod_batch"    : self.prodBatch   ,
            "rank"          : self.rank   ,
            "batch_no"      : self.batchno,
            "qty"           : self.qty   ,
            "date"          : self.tbDate.get()   ,
            "remark"        : self.tbRemark.get()   ,
            "tracking"      : self.tracking   ,
            "partnumber"     : "TBD",
            "typevar": self.typevar
        }
        
        if self.typevar=="Pallet":
         plet = PALLET(
            model       = outData["model"],
            capacity    = outData["capacity"],
            prod_batch  = outData["prod_batch"],
            rank        = outData["rank"],
            qty         = outData["qty"],
            date        = outData["date"],
            remark      = outData["remark"],
            partnumber  = outData["partnumber"],
            sapBatch    = outData["prod_batch"]+outData["rank"],
            tracking    = outData["tracking"]
         )

         plet.create_pdf()
         os.startfile("pdf\\pallet.pdf")
        elif self.typevar=="Inner":
          inn = INNER(
            model=outData["model"],
            capacity=outData["capacity"],
            batch_no=outData["batch_no"],
            rank=outData["rank"],
            qty=outData["qty"],
            date=outData["date"],
            remark=outData["remark"],
            tracking=outData["tracking"]
         )

          inn.create_pdf()
          os.startfile("pdf\\inner.pdf")

        elif self.typevar=="Outer":
          out = OUTER(
             model=outData["model"],
              capacity=outData["capacity"],
              batch_no=outData["batch_no"],
              rank=outData["rank"],
              qty=outData["qty"],
              date=outData["date"],
              remark=outData["remark"],
              partnumber=outData["partnumber"],
              sapBatch=outData["batch_no"] + outData["rank"],
              tracking=outData["tracking"]
            )
          out.create_pdf()
          os.startfile("pdf\\outer.pdf")
        
    
