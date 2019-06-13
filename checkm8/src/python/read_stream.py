import sys
import urllib.request as ur

stream = ur.urlopen(sys.argv[1] + "/shot.jpg")

with open("images/chessboard.jpg", "wb") as output:
  output.write(stream.read())
