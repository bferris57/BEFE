#!/usr/bin/python
#coding=utf8
#-------------------------------------------------------------------------------
#
# Sportr "python challenge"
#
# Command Line: python sportr.py <filename.txt # Read urls from stdin
#
#                 or
#
#               python sportr.py test          # Perform internal test
#
#
# Notes: The function GetFromUrl(url) needs error handling. Hasn't been
#        decided but suggest putting an 'error' tag in the result output
#        to indicate errors like "404 NotFound" etc.
#
#        Wasn't clear which images were "the best url and caption" so
#        I chose to use <meta property="og:url"...>, "og:title", and
#        "og:image".
#
#        Easily changed below in the future
#

from sys            import stdin
from sys            import argv
from urllib.request import urlopen
from html.parser    import HTMLParser

import json

def attrs2dict(attrs):

  dict = {}
  for attrValue in attrs:
    k,v = attrValue
    dict[k] = v
  return dict
    
class myParser(HTMLParser):
  def __init__(self,url):

    HTMLParser.__init__(self)
    self.count            = 0
    self.url              = url
    self.title            = None
    self.metaurl          = None
    self.metatitle        = None
    self.dict             = {}
    self.dict['headline'] = None
    self.dict['images']   = []
    self.images           = []
    
  def handle_starttag(self, tag, attrs):
    dict = attrs2dict(attrs)
    if tag == 'img':
      self.count += 1
    elif tag == 'meta' and 'property' in dict:
      property = dict['property']
      if property == "og:url":
        self.metaurl = dict['content']
      elif property == "og:title":
        if self.title is None:
          self.title = dict['content']
        self.metatitle = dict["content"]
      elif property == "og:image":
        elm = {'url':self.metaurl, 'caption':self.metatitle,'image':dict['content']} 
        self.images.append(elm)

def GetFromUrl(url):

  # Get the page...    
  page = urlopen(url)
  contents = str(page.read()) # Had to convert to string for HTMLParser to work properly, yuck!!!
  dict = {}
  dict['url'] = url
  
  # Pull the page apart...  
  parser = myParser(url)
  parser.feed(contents)
  
  # If all there, put into dict and output...
  return {"url":parser.url,"headline":parser.title,"images":parser.images}
  
if __name__ == '__main__':

  # If arguments passed, process them...
  if len(argv) > 1:
  
    if argv[1] == 'test':
    
      print("Test...")
      output = GetFromUrl("http://www.bbc.co.uk/sport/tennis/37268846")
      numErrors = 0
      if 'url' not in output:
        print("ERROR: Expected 'url' in output")
        numErrors += 1
      if 'headline' not in output:
        print("ERROR: Expected 'headling' in output")
        numErrors += 1
      if 'images' not in output:
        print("ERROR: Expected 'images' in output")
        numErrors += 1
      else:
        images = output['images']
        if len(images) != 1:
          print("ERROR: Expected 1 image in 'images', got %d"%len(images))
          numErrors += 1
      print("Test: %d errors"%numErrors)
      
    else:
    
      print("Unknown option '"+argv[1]+"', expected 'test'")
  
  #
  # No arguments, get urls from stdin...
  #
  
  else:
  
    lines = stdin.readlines()
    lineNo = 0
    output = []

    for line in lines:

      lineNo += 1

      # Get rid of trailing '\n'...
      if line[-1] == '\n':
        line = line[:-1]

      # Process url...
      answer = GetFromUrl(line)
      output.append(answer)
      
    print(json.dumps(output))