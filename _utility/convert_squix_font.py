#!/usr/bin/python
# This script converts fonts generated at http://oleddisplay.squix.ch using library version >3.0.0 format
import sys
import re

def commentRemover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def getName(text):
    return re.findall(r'const char (.*?)\[\] PROGMEM',text,re.DOTALL)[0]

def getInsideBrackets(text):
    return re.findall(r'{(.*?)}',text,re.DOTALL)[0]

def flipByte(inHex):
  inBin = bin(int(inHex, 16))[2:].zfill(8)
  outBin = inBin [7] + inBin [6] + inBin [5] + inBin [4] + inBin [3] + inBin [2] + inBin [1] + inBin [0]
  #out = "0x" + hex(int(outBin, 2))[2:].zfill(2)
  out = int(outBin, 2)
  return out

filename = sys.argv[1]
f = open(filename,"r")
fullText = f.read()
commentsRemoved = commentRemover(fullText)
name = getName(commentsRemoved)
insideBrackets = getInsideBrackets(commentsRemoved)
removedAllWhiteSpace = ''.join(insideBrackets.split())
data = removedAllWhiteSpace.split(",")
width = int(data[0], 16)
height = int(data[1], 16)
first = int(data[2], 16)
number = int(data[3], 16)
widths = [int(x, 16) for x in data[4:number+4]]
bytes = [x*height/8+1 for x in widths]
indexStart = number + 4
flippedCharacters = list()
for i in bytes:
  indexEnd = indexStart + i
  Hexes = data[indexStart:indexEnd]
  flippedBytes = list()
  for j in Hexes:
    flippedBytes.append(flipByte(j))
  flippedCharacters.append(flippedBytes)
  indexStart = indexEnd
f.close()

filename = filename.split(".")[0]+".c"
f = open(filename,"w")
f.write("#if !defined(SUMO_FNT_LIB_{})\r\n".format(name))
f.write("#define SUMO_FNT_LIB_{}\r\n".format(name))
f.write("#include <stdint.h>\r\n\r\n")
for iii in range(number):
  f.write("const _smCharType image_data_{}_{}[{}]\r\n".format(name,hex(iii+first),bytes[iii]))
  f.write("#if defined(_FORCE_PROGMEM__)\r\n")
  f.write("PROGMEM\r\n")
  f.write("#endif\r\n")
  f.write("= { ")
  sarr = ["0x" + hex(a)[2:].zfill(2) for a in flippedCharacters[iii]]
  f.write(", ".join(sarr))
  f.write(" };//character: '")
  f.write("{}'\r\n\r\n".format(chr(iii+first)))
  f.write("const tImage {}_{}\r\n".format(name,hex(iii+first)))
  f.write("#if defined(_FORCE_PROGMEM__)\r\n")
  f.write("PROGMEM\r\n")
  f.write("#endif\r\n")
  f.write("= { image_data_")
  f.write("{}_{}, {}, {}".format(name,hex(iii+first),widths[iii],bytes[iii]))
  f.write("};//character: '")
  f.write("{}' (charW,total bytes)\r\n\r\n".format(chr(iii+first)))
f.write("const tChar {}".format(name))
f.write("_array[] = {")
characterList = list()
for iii in range(number):
  one = "\r\n  // character: '{}'\r\n".format(chr(iii+first))
  two = "  {"
  three = "{0}, &{1}_{0}".format(hex(iii+first),name)
  four = "}"
  characterList.append(one+two+three+four)
f.write(", ".join(characterList))  
f.write("\r\n};\r\n\r\n")
f.write("const tFont {} = ".format(name))
f.write("{ ")
f.write("{}, {}_array, RLE_proportional, {}, 0, RLE_no, 0 ".format(number,name,height))
f.write("};\r\n")
f.write("#endif\r\n")
f.close
