from tkinter     import *
from tkconstants import *
from error       import *

ValidColourTypes = (str, tuple)

#
# Class: Pane
#
# Methods: __init__
#          Clear
#          SetCharacter
#          GetCharacter
#

class Console(Frame):

  def event_mouse(self,event):

    if event.type != EventMotion:
      print("Event: %s (button %d)"%(EventName[event.type],event.num))
      if event.type == EventButtonPress:
        self.ScrollUp()
    return "break"

  def event_keyboard(self,event):

    print("Event: %s (%d = %s)"%(EventName[event.type],event.keysym_num,event.char))

  def event_focus(self,event):

    print("Event: %s"%EventName[event.type])
    return 'break'

  def __init__(self,master=None,w=80,h=24,bg=LightGrey,fg=Black):

    if type(w) != int or type(h) != int:
      raise Error("Expected w,h to be integers")
    if type(bg) not in ValidColourTypes or type(fg) not in ValidColourTypes:
      raise Error("Expected bg,fg to be strings or tuples")

    Frame.__init__(self, master)

    w = max(w,1)
    h = max(h,1)

    self.w = w
    self.h = h
    self.bg = bg
    self.fg = fg

    self.curline = 0
    self.curcol  = 0

    self.lines = [w*' ']*h
    text = self.text = Text(self)
    text['font']   = ('Courier New',10)
    text['fg']     = fg
    text['bg']     = bg
    text['height'] = self.h
    text['width']  = self.w
    text['borderwidth'] = 2
    #text['relief'] = SUNKEN
    text['relief'] = RIDGE
    text.grid()
    self.Clear()

    text.bind(EventName[EventButtonPress]  ,self.event_mouse)
    text.bind(EventName[EventButtonRelease],self.event_mouse)
    text.bind(EventName[EventMotion]       ,self.event_mouse)
    text.bind(EventName[EventEnter]        ,self.event_focus)
    text.bind(EventName[EventLeave]        ,self.event_focus)

    self.bind(EventName[EventKeyPress]     ,self.event_keyboard)
    self.bind(EventName[EventKeyRelease]   ,self.event_keyboard)

  def Refresh(self):

    val = ''
    for line in self.lines:
      val = val + line
    text = self.text
    #text.config(state=NORMAL)
    text.delete(0.0,END)
    text.insert(END,val)
    #text.config(state=DISABLED)

  def Clear(self,colno=None,lineno=None):

    self.SetChar(' ',colno,lineno)

  def SetChar(self,char,colno = None,lineno = None):

    if type(char) != str or len(char) != 1:
      raise Error("Expected char to be a single character string")
    if lineno != None and type(lineno) != int:
      raise Error("Expected lineno to be None or integer")
    if colno != None and type(colno) != int:
      raise Error("Expected colno to be None or integer")

    # If nothing specified, clear the lot
    if lineno == None and colno == None:
      self.lines = [self.w*char]*self.h
    # Have one or both of lineno and colno...
    else:
      if lineno and (lineno < 0 or lineno >= self.h):
        return
      if colno and (colno < 0 or colno >= self.w):
        return
      # Have lineno...
      if lineno != None:
        if colno != None:
          self.lines[lineno] = self.lines[lineno][0:colno] + char + self.lines[lineno][colno+1:]
        else:
          self.lines[lineno] = char*self.w
      # No lineno, clear column in all lines
      else:
        for y in range(0,self.h):
          self.lines[y] = self.lines[y][0:colno] + char + self.lines[y][colno+1:]

    self.Refresh()

    return

  def GetChar(self,colno,lineno):

    if type(colno) != int or type(lineno) != int:
      raise Error("Expected lineno/colno to be integers")
    if type(char) != str or len(char) != 1:
      raise Error("Expected char to be a single character string")

    if colno  < 0 or colno  >= self.w: return
    if lineno < 0 or lineno >= self.h: return
    return self.lines[lineno][colno]

  def ScrollUp(self):

    print("**Scrolling Up 1 line**")
    self.lines = self.lines[1:self.h-1] + [' '*self.w]
    self.Refresh()

  def ScrollDown(self):

    self.lines = [' '*self.w] + self.lines[0:self.h-1]
    self.Refresh()

  def Timer(self):

    print("**Timer**")
    self.after(1000,self.Timer)

  def Print(self,indent = ''):

    # Palette: ┌─┐│ │└─┘

    top = indent + '┌' + self.w*'─' + '┐'
    bot = indent + '└' + self.w*'─' + '┘'

    print(top)
    for i in range(0,self.h):
      print(indent + '│'+self.lines[i]+'|')
    print(bot)

def TestConsole():

  print("Testing Console...")
  indent = '  '

  root = Tk()
  con = Console(root,80,26)
  con.pack()
  con.SetChar('x',5,5)
  con.SetChar('y',4,4)
  con.SetChar('y',3,3)
  con.Print(indent)
  con.after(1000,Console.Timer,con)
  con.Refresh()
  root.mainloop()

if __name__ == '__main__':

  TestConsole()