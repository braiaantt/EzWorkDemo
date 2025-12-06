#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include <QSqlDatabase>
#include "productdao.h"
#include "logdao.h"
#include "logdata.h"

class ProductService
{
public:
    ProductService(QSqlDatabase &db, ProductDao &productDao, LogDao &logDao);
    bool updateProduct(int productId, const QString &newName, int newStock, int newPrice, const QVector<LogData> &logs) const;

    //------ Getters ------
    ProductDao* getProductDao() const;

private:
    QSqlDatabase &db;
    ProductDao &productDao;
    LogDao &logDao;
};

#endif // PRODUCTSERVICE_H
