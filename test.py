import pyotp
import base64
secret = "mysecretkeyhere".encode( "UTF-8" )
b32Secret = base64.b32encode( secret )
totp = pyotp.TOTP( b32Secret ).now()
print( totp )
