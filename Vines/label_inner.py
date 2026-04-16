from reportlab.pdfgen import canvas
from barcode import Code128
from barcode.writer import ImageWriter
import PIL
from PIL import Image
import cv2

class INNER():
    def __init__(self, model, capacity, batch_no, rank, qty, date, remark, tracking):
        self.model = model
        self.capacity = capacity
        self.batch_no = batch_no
        self.rank = rank
        self.qty = qty
        self.date = date
        self.remark = remark
        self.tracking = tracking
        self.baseX = 575
        self.baseY = 579
    
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
        code128_image.save('img\\inner_barcode')
        self.crop("inner_barcode.png",272,280,26,189,260,9)

    def create_model_text(self, can):
    #Create model text
        ind = 0
        lineSpace = 0

        baseHeight = 269
        if len(self.model) > 8:
            baseHeight += 18

        while ind < len(self.model):
            can.drawString(132,  baseHeight- lineSpace, self.model[ind:ind+8])
            ind += 8
            lineSpace += 18
    
    def create_batchno_text(self, can):
    #Create batchno text
        ind = 0
        lineSpace = 0

        baseHeight = 225
        if len(self.batch_no) > 8:
            baseHeight += 18

        while ind < len(self.batch_no):
            can.drawString(128,  baseHeight- lineSpace, self.batch_no[ind:ind+8])
            ind += 8
            lineSpace += 18
    # Tạo PDF 
    def create_pdf(self):
        img_file = 'img\\inner.png'
        pdf_file = 'pdf\\inner.pdf'
        self.create_barcode()
    # Đọc kích thước ảnh label
        img = cv2.imread(img_file)
        img_height, img_width, _ = img.shape
        can = canvas.Canvas(pdf_file, pagesize=(img_width, img_height))
        can.drawImage(img_file, 0, 0, width=img_width, height=img_height)

    #In chuỗi lên PDF
        can.setFont("Helvetica", 16)
        self.create_batchno_text(can) 
        self.create_model_text(can)
        can.setFont("Helvetica", 20)
        # can.drawString(132, 278, self.model)
        can.drawString(403, 278, self.capacity)
        # can.drawString(128, 235, self.batch_no) 
        can.drawString(480, 227, self.rank)
        can.drawString(132, 190, self.qty)
        can.drawString(394, 190, self.date)
        can.drawString(235, 149, self.remark)
        can.drawString(211, 29, self.tracking)
    # In hình ảnh mã barcode
        can.drawImage('img\\inner_barcode.png', 120, 55, width=445, height=70)
        can.showPage()
        can.save()






