#include <QtTest>

class TestQString final : public QObject {
    Q_OBJECT

  private slots:
    static void toUpper();
};

void TestQString::toUpper() {
    const QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}

QTEST_MAIN(TestQString)
#include "TestQString.moc"