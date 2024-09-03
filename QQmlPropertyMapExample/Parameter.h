#include <QObject>
#include <QVariant>

class Parameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit Parameter(QObject *parent = 0){  }
    
    bool enabled() const { return m_enabled; }  
    void setEnabled(bool enabled) { m_enabled = enabled; emit enabledChanged(); }
    
    QVariant value() const { return m_value; }
    void setValue(const QVariant &value) { m_value = value; emit valueChanged(); }

signals:
    void enabledChanged();
    void valueChanged();

private:
    bool m_enabled = false;
    QVariant m_value = QVariant();

};