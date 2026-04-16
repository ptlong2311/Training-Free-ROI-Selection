from reportlab.pdfgen import canvas
from barcode import Code128
from barcode.writer import ImageWriter
from pdf417 import encode, render_image
import cv2

class OUTER():
    def __init__(self, model, capacity, batch_no, rank, qty, date, remark, partnumber,sapBatch, tracking):
        self.model = model
        self.capacity = capacity
        self.batch_no = batch_no
        self.rank = rank
        self.qty = qty
        self.date = date
        self.remark = remark
        self.partnumber = partnumber
        self.sapBatch = sapBatch
        self.tracking = tracking
        self.baseX = 575
        self.baseY = 579

    def axis_y(self, y):
        return self.baseY - y
    #Crop
    def crop(self, imgName, bx, by, ix1,iy1, ix2, iy2):
        img = cv2.imread("img\\"+ imgName)
        x1, y1 = ix1, by-iy1
        x2, y2 = ix2, by-iy2-80
        cropped_img = img[y1:y2, x1:x2]
        cv2.imwrite("img\\" + imgName, cropped_img)
    #Tạo mã barcode và lưu thành hình ảnh
    def create_barcode(self):
        code128_image = Code128(self.tracking, writer=ImageWriter())
        code128_image.save('img\\outer_barcode')
        self.crop("outer_barcode.png", 272, 280, 26, 189, 250, 9)
    #Tạo mã pdf417 và lưu thành hình ảnh
    def create_pdf417_code(self):
        barcode_data = encode(self.sapBatch)
        image = render_image(barcode_data, scale=2)
        image.save('img\\outer_pdf417.png', 'PNG')
        img = cv2.imread('img\\outer_pdf417.png')
        x1, y1 = 20, 58-37
        x2, y2 = 362, 58-20
        cropped_img = img[y1:y2, x1:x2]
        cv2.imwrite('img\\outer_pdf417.png', cropped_img)
    #Scale
    def scale_pdf417(self, scale):
        width = 382
        height = 58
        return {
            "width": round(width*scale),
            "height": round(height*scale)
        }
    
    def create_model_text(self, can):
    #Create model text
        ind = 0
        lineSpace = 0

        baseHeight = 274
        if len(self.model) > 8:
            baseHeight += 18

        while ind < len(self.model):
            can.drawString(106,  baseHeight- lineSpace, self.model[ind:ind+8])
            ind += 8
            lineSpace += 18

    def create_batchno_text(self, can):
    #Create batchno text
        ind = 0
        lineSpace = 0

        baseHeight = 214
        if len(self.batch_no) > 8:
            baseHeight += 18

        while ind < len(self.batch_no):
            can.drawString(106,  baseHeight- lineSpace, self.batch_no[ind:ind+8])
            ind += 8
            lineSpace += 18
    def create_sapbatch_text(self, can):
    #Create sapbatch text
        ind = 0
        lineSpace = 0

        baseHeight = 200
        if len(self.batch_no+self.rank) > 7:
            baseHeight += 18

        while ind < len(self.batch_no+self.rank):
            can.drawString(183,  baseHeight- lineSpace, self.sapBatch[ind:ind+7])
            ind += 7
            lineSpace += 18
    
    # Tạo PDF 
    def create_pdf(self):
       img_file = 'img\\outer.png'
       pdf_file = 'pdf\\outer.pdf'
       self.create_barcode()
       self.create_pdf417_code()
       img = cv2.imread(img_file)
       img_height, img_width, _ = img.shape

    

    # Tạo tệp PDF với kích thước bằng với kích thước ảnh label
       can = canvas.Canvas(pdf_file, pagesize=(img_width, img_height))
       can.drawImage(img_file, 0, 0, width=img_width, height=img_height)
    # In các chuỗi lên tệp PDF
       can.setFont("Helvetica-Bold", 80)
       can.drawString(493, 214, self.rank)
       can.setFont("Helvetica", 14)
    #    can.drawString(110, 281, self.model)
       self.create_model_text(can)
       can.drawString(290, 281, self.capacity)
    #    can.drawString(106, 214, self.batch_no)  
    #    can.drawString(183, 200, self.batch_no + self.rank)
       self.create_batchno_text(can)
       self.create_sapbatch_text(can)
       
       can.drawString(383, 214, self.rank)
       can.drawString(118, 135, self.qty)
       can.drawString(295, 135, self.date)
       can.drawString(110, 85, self.remark)
       can.drawString(350, 72, self.partnumber)
       can.setFont("Helvetica", 10)

       can.drawString(125, 11, self.tracking)
    # In hình ảnh mã barcode
       can.drawImage('img\\outer_barcode.png', 100, 21, width=175, height=32)
    # In hình ảnh mã PDF417
       can.drawImage('img\\outer_pdf417.png', 410, 21, width=156 ,height=32)
       can.drawImage('img\\outer_pdf417.png', 183, 214, width=80 ,height=25)
       can.showPage()
       can.save()
    
    
