#include <QCoreApplication>
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <cmath>

// Base32 kod çözme fonksiyonu (harici bir kütüphane veya kendi implementasyonunuz gerekebilir)
QByteArray base32Decode(const QString& base32) {
    // Bu fonksiyonun gerçek implementasyonu gereklidir.
    // Qt'de hazır bir Base32 çözücü olmayabilir, bu yüzden
    // harici bir kütüphane kullanabilir veya kendiniz implemente edebilirsiniz.
    // Örnek bir harici kütüphane: https://github.com/crockford/base32
    QByteArray decoded;
    // ... Base32 çözme işlemleri ...
    Q_UNUSED(base32); // Şu an kullanılmıyor
    return decoded;
}

QString generateTotp(const QByteArray& secretKey) {
    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    qint64 timestamp = currentDateTime.toSecsSinceEpoch();
    qint64 step = 30;
    qint64 counter = floor(timestamp / step);

    QByteArray counterBytes;
    for (int i = 7; i >= 0; --i) {
        counterBytes.append(static_cast<char>(counter & 0xFF));
        counter >>= 8;
    }

    QCryptographicHash hmac(QCryptographicHash::Sha1);
    hmac.addData(secretKey);
    hmac.addData(counterBytes);
    QByteArray hash = hmac.result();

    int offset = hash.at(hash.size() - 1) & 0x0F;

    qint32 truncatedHash = 0;
    for (int i = 0; i < 4; ++i) {
        truncatedHash <<= 8;
        truncatedHash |= (hash.at(offset + i) & 0xFF);
    }

    truncatedHash &= 0x7FFFFFFF; // En anlamlı biti sıfırla

    qint32 otp = truncatedHash % 1000000; // 6 haneli kod

    return QString::number(otp).rightJustified(6, '0');
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Kullanıcının gizli anahtarı (Base32 ile kodlanmış)
    QString base32Secret = "JBSWY3DPEHPK3PXP"; // Örnek bir anahtar

    // Base32 kodunu çöz
    QByteArray secretKey = base32Decode(base32Secret);

    if (secretKey.isEmpty()) {
        qDebug() << "Hata: Base32 kodu çözülemedi.";
        return 1;
    }

    // OTP kodunu üret
    QString otpCode = generateTotp(secretKey);
    qDebug() << "Üretilen OTP Kodu:" << otpCode;

    return a.exec();
}
