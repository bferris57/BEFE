import Tkinter
from PIL import ImageTk, Image, ImageDraw

if False:

    def __init__(self):
      Tkinter.Tk.__init__(self)

      self.configure(bg='red')

      self.ImbImage = Tkinter.Canvas(self, highlightthickness=0, bd=0, bg='blue')
      self.ImbImage.pack()

      im = Image.open(r'folder.png')

      im = ImageDraw.Draw(im)

      im = im.point((0, 0), fill="red")

      self.i = ImageTk.PhotoImage(im)
      self.ImbImage.create_image(139, 59, image=self.i)




if True:

  class image_manip(Tkinter.Tk):

    def __init__(self):
      Tkinter.Tk.__init__(self)
      #self.configure(bg='red')s
      im = Image.open('folder.png')
      width, height = im.size
      self.ImbImage = Tkinter.Canvas(self, highlightthickness=0, bd=0, bg='red', width=width, height=height)
      self.ImbImage.grid(row=0,column=0)

      #draw = ImageDraw.Draw(im)
      #draw.rectangle([0, 0, 40, 40 ],  fill="green")
      #del draw

      self.i = ImageTk.PhotoImage(im)
      self.ImbImage.create_image(width/2, height/2, image=self.i)
      self.text = Tkinter.Text()
      self.text['height'] = 1
      self.text.grid(row=0,column=1)
      self.text.insert(Tkinter.INSERT,"Hi there")

def run():
    image_manip().mainloop()
if __name__ == "__main__":
    run()
