#ifndef TABLEWIDGETSALES_H
#define TABLEWIDGETSALES_H

#include <QTableWidget>
#include "saleinfo.h"

enum class DateTimeMode{
    DateTime = 0,
    OnlyTime = 1
};

class TableWidgetSales : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidgetSales(QWidget *parent = nullptr);
    //------ Constants ------
    static inline constexpr int LIMIT_LOAD_SALES = 10;

    //------ Public Methods ------
    void loadSales(const QVector<SaleInfo> &sales);
    void reset();

    //------ Setters ------
    void setDateTimeMode(DateTimeMode showMode);

signals:
    void saleClicked(int saleId);
    void getMoreSales(int currentSalesCount);

private slots:
    void onItemPressed(QTableWidgetItem *item);
    void scrollBarValueChanged(int value);

private:
    QVector<SaleInfo> sales;
    bool requestMoreSales;
    int loadedSales;
    int _rowCount;
    DateTimeMode dateTimeMode;

    //------ Initialization ------
    void connectSignalsAndSlots();
    void initTableWidget();

    //------ Utils ------
    void addSaleItems(int rowCount, const QString &id, const QString &saleCount, const QString &time, const QString &total);
    void addProductItems(int rowCount, const QString &productName, const QString &productAmount, const QString &productPrice, const QString &subTotal);

    //------ Helpers ------
    QString getTime(const QDateTime &dateTime) const;

    //------ Enums ------

    enum class SaleColumn{
        Id = 0,
        Count = 1,
        Time = 2,
        TotalText = 3,
        Total = 4
    };

    enum class ProductColumn{
        Name = 1,
        Amount = 2,
        Price = 3,
        SubTotal = 4
    };
};

#endif // TABLEWIDGETSALES_H
