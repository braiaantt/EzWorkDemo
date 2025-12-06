#ifndef PRODUCTDAO_H
#define PRODUCTDAO_H

#include <QSqlDatabase>
#include "cartproduct.h"
#include "product.h"

class ProductDao
{
public:
    ProductDao(QSqlDatabase &db);

    std::shared_ptr<Product> addProduct(const QString &barCode, const QString &name, int stock, int price, ProductType type) const;
    bool updateStockProducts(const QVector<CartProduct> &products) const;
    bool updateProduct(int productId, const QString &newName, int newStock, int newPrice) const;
    bool deleteProduct(int productId) const;
    QVector<std::shared_ptr<Product>> getProducts() const;

private:
    QSqlDatabase &db;
};

#endif // PRODUCTDAO_H
