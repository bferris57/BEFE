import sys
pythonver = sys.version.split()[0]
if pythonver == '2.7':
  from Tkinter import *
elif pythonver >= '3':
  from tkinter import *
else:
  raise RuntimeError("Don't know where to get tkinter in Python "+pythonver)

from PIL   import Image,ImageTk
from error import *

class IconsAndText(Frame):

  def __init__(self,parent=None,text='',file='',files=[]):

    Frame.__init__(self,parent)
    self.grid()

    self.images      = []
    self.photoimages = []
    self.icons       = {}
    self.text        = text
    self.tnsize      = 16
    self.xsize       = 40*self.tnsize
    self.ysize       = self.tnsize
    if type(files) not in (list,tuple):
      raise Error("Expected files to be a list or tuple")
    if type(file) != str:
      raise Error("Expected file to be a file name string")
    files = list(files)
    if file:
      files.append(file)
    x = 0
    y = 0
    width = self.xsize
    height = self.ysize
    canvas = self.canvas = Canvas(self, highlightthickness=0, bg='white',bd=0,width=width,height=height)
    for file in files:
      print("Processing %s..."%file)
      im = Image.open(file)
      im.thumbnail((self.tnsize,self.tnsize))
      self.images.append(im)
      pi = ImageTk.PhotoImage(im)
      self.photoimages.append(pi)
      width, height = im.size
      self.canvas.create_image(x,y,image=pi,anchor=NW)
      x = x + self.tnsize
    if text:
      canvas.create_text(x,y,text=self.text,anchor=NW,font="Courier 10")
    canvas.pack()
    #print("text col =",col)

class Application(Frame):

  def buttonpressed(self):

    print("Button pressed")
    self.quit()

  def __init__(self,parent=None):

    Frame.__init__(self,parent)

    print("Application.__init__: Creating line")
    self.pack()
    self.lines = []

    it = IconsAndText(self,text="Hello world!",file=r"d:\icons\basic\folder.png")
    self.lines.append(it)
    it.grid(row=0,column=0)

    it = IconsAndText(self,text="Hello again",files=[r"d:\icons\basic\folderx.png",r"d:\icons\basic\folder.png"])
    self.lines.append(it)
    it.grid(row=1,column=0)

    it = IconsAndText(self,text="Hello again",files=[r"d:\icons\basic\empty.png",r"d:\icons\basic\folderx.png"])
    self.lines.append(it)
    it.grid(row=2,column=0)

    it = IconsAndText(self,text="Helloy helloy!",files=[r"d:\icons\basic\empty.png",r"d:\icons\basic\empty.png",r"d:\icons\basic\folderx.png"])
    self.lines.append(it)
    it.grid(row=3,column=0)


    b = Button(self)
    b["text"] = "QUIT"
    b["fg"] = "red"
    b["command"] = self.buttonpressed
    self.but = b
    self.but.grid(row=4,column=0)

    #label = Label(image=photo)
    #label.image = photo # keep a reference!
    #label.pack()
    #self.label = label

if __name__ == '__main__' and True:
  root = Tk()
  app = Application()
  app.pack()
  root.mainloop()

