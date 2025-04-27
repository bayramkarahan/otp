#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>
#include <openssl/hmac.h>
#include <cmath>
#include <bitset>

std::string base32Encode(const std::string& input) {
    const std::string base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string binaryString = "";

    for (char c : input) {
        binaryString += std::bitset<8>(c).to_string();
    }

    std::string base32 = "";
    for (size_t i = 0; i < binaryString.length(); i += 5) {
        std::string chunk = binaryString.substr(i, 5);
        chunk.append(5 - chunk.length(), '0'); // pad with '0' to ensure chunk is 5 bits
        int index = std::stoi(chunk, nullptr, 2);
        base32 += base32Chars[index];
    }

    return base32;
}
// BASE32 decode fonksiyonu
QByteArray base32Decode(const QString &base32)
{
    QByteArray result;
    int buffer = 0, bitsLeft = 0;
    QString chars = base32.toUpper();

    for (QChar c : chars) {
        if (c == '=' || c.isSpace()) continue;

        int val = -1;
        if (c >= 'A' && c <= 'Z') val = c.unicode() - 'A';
        else if (c >= '2' && c <= '7') val = c.unicode() - '2' + 26;

        if (val < 0) continue;

        buffer = (buffer << 5) | val;
        bitsLeft += 5;

        if (bitsLeft >= 8) {
            result.append((buffer >> (bitsLeft - 8)) & 0xFF);
            bitsLeft -= 8;
        }
    }
    return result;
}

// TOTP üretici
QString generateTOTP(const QByteArray &key, qint64 timestamp = QDateTime::currentSecsSinceEpoch(), int timeStep = 60)
{
    qint64 counter = timestamp / timeStep;

    QByteArray timeBytes(8, 0);
    for (int i = 7; i >= 0; --i) {
        timeBytes[i] = counter & 0xFF;
        counter >>= 8;
    }

    unsigned int len = 0;
    unsigned char *hmac = HMAC(EVP_sha1(), key.data(), key.size(),
                               reinterpret_cast<const unsigned char*>(timeBytes.data()), timeBytes.size(),
                               nullptr, &len);

    int offset = hmac[19] & 0x0F;
    int bin_code = ((hmac[offset] & 0x7F) << 24) |
                   ((hmac[offset + 1] & 0xFF) << 16) |
                   ((hmac[offset + 2] & 0xFF) << 8) |
                   (hmac[offset + 3] & 0xFF);

    int otp = bin_code % 1000000; // 6 haneli TOTP
    return QString("%1").arg(otp, 6, 10, QLatin1Char('0'));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream out(stdout);
    QTextStream in(stdin);

    QString text = "qrkilit-0000001";

    std::string key=base32Encode(text.toStdString());
    qDebug()<<"key:"<<key.c_str();
    QByteArray secret = base32Decode(key.c_str());
    qDebug()<<"secret:"<<secret;

    QString otp = generateTOTP(secret, QDateTime::currentSecsSinceEpoch(), 30);
    out << "Şu anki TOTP kodu: " << otp << Qt::endl;

    out << "Kod girin (Authenticator uygulamasındaki): ";
    out.flush();
    QString inputCode = in.readLine();

    if (inputCode == otp) {
        out << "✅ Doğrulama başarılı!" << Qt::endl;
    } else {
        out << "❌ Kod geçersiz!" << Qt::endl;
    }

    return 0;
}

