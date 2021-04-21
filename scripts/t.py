#------------------------------------------------------------------------------
#
# Functions: red   - Print red error
#            error - Print red error and exit with status
#            green - Print green message
#

def red(msg):

  print(color("ERROR: "+msg,fore='red',style='bright'))

def error(msg):

  red(msg)
  sys.exit(1)

def green(msg):

  print(color(msg,fore='green',style='bright'))

# Function: color - Use colr.color if stdout is a tty
#

def color(*args,**kwargs):

  if not sys.stdout.isatty():
    return args[0]
  return colr.color(*args,**kwargs)
