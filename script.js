function base32Encode(input) {
    const base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    let binaryString = "";
    
    for (let i = 0; i < input.length; i++) {
        binaryString += input.charCodeAt(i).toString(2).padStart(8, '0');
    }

    let base32 = "";
    for (let i = 0; i < binaryString.length; i += 5) {
        const chunk = binaryString.substr(i, 5).padEnd(5, '0');
        const index = parseInt(chunk, 2);
        base32 += base32Chars[index];
    }

    return base32;
}

const text = "qrkilit-555666";
const encodedText = base32Encode(text);
console.log(encodedText);
alert(encodedText);

const jsotp = require('jsOTP');

// TOTP nesnesi oluşturma ve doğrulama
const totp = new jsotp.TOTP(encodedText);
const token = totp.now(); // Mevcut zaman için OTP hesapla
console.log(token); // Örneğin: 789123
alert(token);


