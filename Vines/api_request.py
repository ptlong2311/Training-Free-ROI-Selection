from label_data import LabelData
from tkinter import messagebox
import tkinter as tk
from tkinter import ttk
import sys
from check_packaging import check_packaging

class RequestApi:
    #Tạo bảng request
    def __init__(self, root, callback):
        self.pdf_button_frame = tk.Frame(root)
        self.dialogue = tk.Toplevel(self.pdf_button_frame)
        self.dialogue.title("LABEL CHECKING")
        # self.dialogue.geometry("400x300")
        self.callback = callback
        x = root.winfo_x()
        y = root.winfo_y()
        self.dialogue.geometry("+%d+%d" % (x , y ))

        # self.root.eval('tk::PlaceWindow . center')
        # self.dialogue.pack(pady=20)
        
        self.type_label = tk.Label(self.dialogue, text="Type:")
        self.type_label.grid(row=0, column=0, padx=10, pady=5)
        
        self.cbType = ttk.Combobox(self.dialogue, values=["Inner", "Outer", "Pallet"])
        self.cbType.set("")
        self.cbType.grid(row=0, column=1, padx=10, pady=5)
        self.cbType.bind("<<ComboboxSelected>>", self.update_boxid_entry)
        self.cbType.current(1)

        self.lbBarcode = tk.Label(self.dialogue, text="Barcode:")
        self.lbBarcode.grid(row=1, column=0, padx=10, pady=5)
        
        self.tbBarcode = tk.Entry(self.dialogue)
        self.tbBarcode.grid(row=1, column=1, padx=10, pady=5)

        self.btRequest = tk.Button(self.dialogue, text="Check Label", command=self.send_request)
        self.btRequest.grid(row=3, columnspan=2, pady=10)
        self.txtResponse = tk.StringVar()
        self.lbTxtResponse = tk.Label(self.dialogue, textvariable=self.txtResponse, wraplength=350)
        self.lbTxtResponse.grid(row=4, columnspan=2, padx=10, pady=5)

        #sua
        self.lbBoxId = tk.Label(self.dialogue, text="BoxID:")
        self.lbBoxId.grid(row=2, column=0, padx=10, pady=5)
        
        self.lbBoxId.grid_forget()

        self.tbBoxid = tk.Entry(self.dialogue)
        self.tbBoxid.grid(row=2, column=1, padx=10, pady=5)
        self.tbBoxid.grid_forget()

        self.dialogue.protocol("WM_DELETE_WINDOW", self.close_window)
    
    def close_window(self):
        sys.exit()


    def update_boxid_entry(self, event=None):
      if self.cbType.get() == "Pallet":
        self.lbBoxId.grid(row=2, column=0, padx=10, pady=5)  
        self.tbBoxid.grid(row=2, column=1, padx=10, pady=5)      
        
      else:
        self.lbBoxId.grid_forget()
        self.tbBoxid.grid_forget()  # Ẩn trường BoxID
      
    # Test API
    def send_request(self):
        typeVal = self.cbType.get()
        barcodeVal = self.tbBarcode.get()
        if not typeVal:
           return messagebox.showwarning(title=None, message="Please select TYPE")
        
        if not barcodeVal:
           return messagebox.showwarning(title=None, message="Please input Barcode value")
        
        if len(barcodeVal)< 12:
           return messagebox.showerror(title=None, message="Barcode length error")
        
        try:
            token = "ManualPacking01"
            result = check_packaging(token, 2, "rqw4fqfqfr")
            print(result)
        except Exception as e:
            self.txtResponse.set("Lỗi gọi API")
            
        if result[0]:
            outData = {
                # "rank"       : "sjdkjd",
                # "qty"        : "dhakjd",
                # "prod_batch" : "sjadj",
                # "batch_no"   : "sjadj",
                "barcode"    : self.tbBarcode.get(),
                "typevar"    : typeVal,
            }
                    
            for data in  result[1]:
                if data["KEY"] == "Barcode":
                   outData["tracking"] = data["VAL"]
                if data["KEY"] == "AssemblyBatch":
                    outData["batch_no"] = data["VAL"]
                    outData["prod_batch"] = data["VAL"]
                if data["KEY"] == "RANK":
                   outData["rank"] = data["VAL"]
                if data["KEY"] == "QTY":
                   outData["qty"] = data["VAL"]
                
            #create labclel data
            self.data_label = LabelData(self.dialogue, outData)

        else:
           self.txtResponse.set(result[1])
        # #fake output data
       

       
