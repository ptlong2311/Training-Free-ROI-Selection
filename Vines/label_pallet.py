from reportlab.pdfgen import canvas
from barcode import Code128
from barcode.writer import ImageWriter
from pdf417 import encode, render_image
import cv2

class PALLET():
    def __init__(self, model, capacity, prod_batch, rank, qty, date, remark, partnumber,sapBatch, tracking):
        self.model = model
        self.capacity = capacity
        self.prod_batch = prod_batch
        self.rank = rank
        self.qty = qty
        self.date = date
        self.remark = remark
        self.partnumber = partnumber
        self.sapBatch = sapBatch
        self.tracking = tracking
        self.baseX = 575
        self.baseY = 579
    
    def axis_y(self,y):
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
        code128_image.save('img\\pallet_barcode')
        self.crop("pallet_barcode.png", 272, 280, 26, 189, 243, 9)
    
    #Tạo mã PDF417 và lưu thành hình ảnh
    def create_pdf417_code(self):
        barcode_data = encode(self.sapBatch)
        image = render_image(barcode_data, scale=2)
        image.save('img\\pallet_pdf417_code.png', 'PNG')
        img = cv2.imread('img\\pallet_pdf417_code.png')
        x1, y1 = 20, 58-37
        x2, y2 = 362, 58-20
        cropped_img = img[y1:y2, x1:x2]
        cv2.imwrite('img\\pallet_pdf417_code.png', cropped_img)
    #Scale
    def scale_pdf417(self, scale):
        width = 382
        height = 58
        return  {
            "width" : round(width*scale),
            "height": round(height*scale)
        }

    def create_model_text(self, can):
    #Create model text
        ind = 0
        lineSpace = 0

        baseHeight = self.axis_y(208)
        if len(self.model) > 8:
            baseHeight += 18

        while ind < len(self.model):
            can.drawString(120,  baseHeight- lineSpace, self.model[ind:ind+8])
            ind += 8
            lineSpace += 18
    def create_prodbatch_text(self, can):
    #Create prodbatch text
        ind = 0
        lineSpace = 0

        baseHeight = 300
        if len(self.model) > 8:
            baseHeight += 18

        while ind < len(self.prod_batch):
            can.drawString(120,  baseHeight- lineSpace, self.prod_batch[ind:ind+8])
            ind += 8
            lineSpace += 18
    def create_sapbatch_text(self, can):
    #Create sapbatch text
        ind = 0
        lineSpace = 0

        baseHeight = 216
        if len(self.prod_batch+self.rank) > 7:
            baseHeight += 18

        while ind < len(self.prod_batch+self.rank):
            can.drawString(336,  baseHeight- lineSpace, self.sapBatch[ind:ind+7])
            ind += 7
            lineSpace += 18

    # Tạo PDF 
    def create_pdf(self):
        img_file = 'img\\pallet.png'
        pdf_file = 'pdf\\pallet.pdf'
        
        # Tạo mã barcode
        self.create_barcode()
        # Tạo mã PDF417 
        self.create_pdf417_code()
        
        # Đọc kích thước ảnh label
        img = cv2.imread(img_file)
        img_height, img_width, _ = img.shape

        # Tạo tệp PDF với kích thước bằng với kích thước ảnh label
        can = canvas.Canvas(pdf_file, pagesize=(img_width, img_height))
        can.drawImage(img_file, 0, 0, width=img_width, height=img_height)

        # In các chuỗi lên tệp PDF
        can.setFont("Helvetica-Bold", 80)
        can.drawString(450, 300, self.rank)
        can.setFont("Helvetica", 15)
        can.drawString(480, 216, self.date)
        can.setFont("Helvetica", 15)
        can.drawString(130, 132, self.remark)
        can.setFont("Helvetica", 18)
        self.create_prodbatch_text(can)
        self.create_sapbatch_text(can)
        self.create_model_text(can)
        can.drawString(330, self.axis_y(207), self.capacity)
        # can.drawString(130, 300, self.prod_batch) 
        can.drawString(345, 300, self.rank)
        can.drawString(130, 216, self.qty)
        can.drawString(420, 129, self.partnumber)
        can.setFont("Helvetica", 13)
        # can.drawString(345, 216, self.prod_batch+ self.rank)
        can.drawString(250, 25, self.tracking)
        # In hình ảnh mã barcode
        can.drawImage('img\\pallet_barcode.png', 150, 44, width=400, height=55)
        # In hình ảnh mã PDF417
        can.drawImage('img\\pallet_pdf417_code.png', 114, self.axis_y(154), width=450 ,height=96)
        can.showPage()
        can.save()



