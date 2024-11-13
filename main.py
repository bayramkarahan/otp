import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import base64
import pyotp
#import array
def base32_encode(data):
    """Encodes binary data using base32 encoding."""
    encoded_data = base64.b32encode(data)
    return encoded_data.decode('utf-8').rstrip('=')  # Remove padding
kurumid="0000001"
#binary_data = b"qrkilit-0000001"
binary_data = bytes("qrkilit-"+str(kurumid), 'utf-8')

#binary_data.extend(str(kurumid))

secret_key = base32_encode(binary_data)
print(secret_key)

# Create a TOTP instance
totp = pyotp.TOTP(secret_key)

class OTPWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="OTP Lock Application")

        self.set_size_request(300, 200)

        # Main
        self.layout = Gtk.Box()
        self.add(self.layout)

        # Layout
        self.grid = Gtk.Grid()
        self.layout.set_center_widget(self.grid)

        # OTP Entry
        self.otp_entry = Gtk.Entry()
        self.otp_entry.set_placeholder_text("Enter OTP")
        self.grid.attach(self.otp_entry, 0, 0, 2, 1)

        # Verify Button
        self.verify_button = Gtk.Button(label="Verify OTP")
        self.verify_button.connect("clicked", self.on_verify_clicked)
        self.grid.attach(self.verify_button, 0, 1, 2, 1)

        # Result Label
        self.result_label = Gtk.Label(label="")
        self.grid.attach(self.result_label, 0, 2, 2, 1)
        #self.fullscreen()

    def on_verify_clicked(self, widget):
        entered_otp = self.otp_entry.get_text()

        print(totp.now())
        if totp.verify(entered_otp):
            self.result_label.set_text("OTP Verified Successfully!")
            self.result_label.set_markup("<span color='green'>OTP Verified Successfully!</span>")
        else:
            self.result_label.set_text("Invalid OTP!")
            self.result_label.set_markup("<span color='red'>Invalid OTP!</span>")

if __name__ == "__main__":
    window = OTPWindow()
    window.connect("destroy", Gtk.main_quit)
    window.show_all()
    Gtk.main()
