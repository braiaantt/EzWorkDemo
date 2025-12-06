#include "tablewidgetsales.h"
#include <QScrollBar>
#include "utils.h"
#include <QHeaderView>

TableWidgetSales::TableWidgetSales(QWidget *parent) :
    QTableWidget(parent),
    requestMoreSales(true),
    loadedSales(0),
    _rowCount(0),
    dateTimeMode(DateTimeMode::OnlyTime)
{
    initTableWidget();
    connectSignalsAndSlots();
}

//------ Initialization ------

void TableWidgetSales::initTableWidget()
{
    setColumnCount(5);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->hideColumn(static_cast<int>(SaleColumn::Id));

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setFocusPolicy(Qt::NoFocus);
}

void TableWidgetSales::connectSignalsAndSlots()
{
    connect(this, &QTableWidget::itemPressed, this, &TableWidgetSales::onItemPressed);
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &TableWidgetSales::scrollBarValueChanged);
}

//------ Public Methods ------

void TableWidgetSales::loadSales(const QVector<SaleInfo> &newSales)
{
    if(newSales.isEmpty()){
        requestMoreSales = false;
        return;
    }

    for(int i = 0; i<newSales.size(); i++){

        const SaleInfo &sale = newSales[i];
        const OperationInfo &operation = sale.getOperationInfo();

        QString id = QString::number(sale.getId());
        QString saleCount = "#" + QString::number(loadedSales+1);
        QString time = getTime(operation.getDateTime());
        QString total = utils::formatPrice(operation.getAmount());

        addSaleItems(_rowCount, id, saleCount, time, total);

        const QVector<CartProduct> &products = sale.getSoldProducts();

        for(int j = 0; j<products.size(); j++){

            _rowCount++;

            const CartProduct &product = products[j];

            QString productName = product.getName();
            QString productAmount = product.getStrAmount();
            QString productPrice = product.getStrPrice();
            QString subTotal = product.getStrSubTotal();

            addProductItems(_rowCount, productName, productAmount, productPrice, subTotal);

        }

        _rowCount++;
        this->insertRow(_rowCount);
        _rowCount++;
        loadedSales++;

    }

    sales.append(newSales);
}

void TableWidgetSales::reset()
{
    _rowCount = 0;
    loadedSales = 0;
    requestMoreSales = true;
    this->setRowCount(_rowCount);
    sales.clear();
}

//------ Utils ------

void TableWidgetSales::addSaleItems(int _rowCount, const QString &id, const QString &saleCount, const QString &time, const QString &total)
{
    QVector<QPair<QString, SaleColumn>> itemsData = {
        {id, SaleColumn::Id},
        {saleCount, SaleColumn::Count},
        {time, SaleColumn::Time},
        {"Total:", SaleColumn::TotalText},
        {total, SaleColumn::Total}
    };

    QColor background(50, 50, 50);
    QColor textColor(225, 225, 225);

    this->insertRow(_rowCount);

    for(const auto& [text, column] : itemsData){
        QTableWidgetItem *item = new QTableWidgetItem(text);
        item->setBackground(background);
        item->setForeground(textColor);
        this->setItem(_rowCount, static_cast<int>(column), item);
    }
}

void TableWidgetSales::addProductItems(int _rowCount, const QString &productName, const QString &productAmount, const QString &productPrice, const QString &subTotal)
{
    QTableWidgetItem *itemProductName = new QTableWidgetItem(productName);
    QTableWidgetItem *itemProductAmount = new QTableWidgetItem(productAmount);
    QTableWidgetItem *itemProductPrice = new QTableWidgetItem(productPrice);
    QTableWidgetItem *itemProductSubTotal = new QTableWidgetItem(subTotal);

    this->insertRow(_rowCount);

    this->setItem(_rowCount, static_cast<int>(ProductColumn::Name), itemProductName);
    this->setItem(_rowCount, static_cast<int>(ProductColumn::Amount), itemProductAmount);
    this->setItem(_rowCount, static_cast<int>(ProductColumn::Price), itemProductPrice);
    this->setItem(_rowCount, static_cast<int>(ProductColumn::SubTotal), itemProductSubTotal);
}

//------ Private Slots ------

//If a product row was selected, this select the Sale row
void TableWidgetSales::onItemPressed(QTableWidgetItem *item)
{
    int row = item->row();

    for(int i = row; i>=0; i--){

        QTableWidgetItem *item = this->item(i, static_cast<int>(SaleColumn::Id));

        if(!item) continue;

        this->selectRow(i);
        int saleId = item->text().toInt();
        emit saleClicked(saleId);
        break;
    }
}

void TableWidgetSales::scrollBarValueChanged(int value)
{
    if(value != 0 &&
        value == this->verticalScrollBar()->maximum() &&
        requestMoreSales)
    {
        emit getMoreSales(sales.size());
    }
}

//------ Helpers ------

QString TableWidgetSales::getTime(const QDateTime &dateTime) const
{
    if(dateTimeMode == DateTimeMode::DateTime){
        return dateTime.toString("dd/MM/yyyy - hh:mm");
    } else {
        return dateTime.toString("hh:mm AP");
    }
}

//------ Setters ------

void TableWidgetSales::setDateTimeMode(DateTimeMode mode)
{
    dateTimeMode = mode;
}

