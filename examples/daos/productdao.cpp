#include "productdao.h"
#include <QSqlQuery>
#include <QSqlError>

ProductDao::ProductDao(QSqlDatabase &db) :
    db(db)
{

}

std::shared_ptr<Product> ProductDao::addProduct(const QString &barCode, const QString &name, int stock, int price, ProductType type) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO products (name, barcode, stock, price, type) VALUES (:name, :barcode, :stock, :price, :type)");

    query.bindValue(":name", name);
    query.bindValue(":barcode", barCode);
    query.bindValue(":stock", stock);
    query.bindValue(":price", price);
    query.bindValue(":type", (int)type);

    if(!query.exec()){
        qWarning()<<"Error al insertar el producto '"<<name<<"': "<<query.lastError();
        return nullptr;
    }

    int id = query.lastInsertId().toInt();
    return std::make_shared<Product>(id, name, barCode, stock, price, type);
}

bool ProductDao::updateStockProducts(const QVector<CartProduct> &products) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE products SET stock = :stock WHERE id = :id");

    for(const CartProduct &product : products){

        query.bindValue(":stock", product.getStock());
        query.bindValue(":id", product.getId());

        if(!query.exec()){
            qWarning()<<"Error al actualizar stock de producto vendido: "<<query.lastError();
            return false;
        }

    }

    return true;
}

bool ProductDao::updateProduct(int productId, const QString &newName, int newStock, int newPrice) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE products SET name = :newName, stock = :newStock, price = :newPrice WHERE id = :productId");
    query.bindValue(":productId", productId);
    query.bindValue(":newName", newName);
    query.bindValue(":newStock", newStock);
    query.bindValue(":newPrice", newPrice);

    if(!query.exec()){
        qWarning()<<"Error al actualizar el producto: Id #"<<productId<<". "<<query.lastError();
        return false;
    }

    return true;
}

bool ProductDao::deleteProduct(int productId) const
{
    QSqlQuery query(db);
    query.prepare("UPDATE products SET deleted = 1 WHERE id = :productId");
    query.bindValue(":productId", productId);

    if(!query.exec()){
        qWarning()<<"Error al eliminar el producto "<<productId<<". "<<query.lastError();
        return false;
    }

    return true;
}

QVector<std::shared_ptr<Product>> ProductDao::getProducts() const
{
    QSqlQuery query(db);
    query.prepare("SELECT id, name, barcode, price, stock, type FROM products WHERE deleted = 0");

    if(!query.exec()){
        qWarning()<<"ProductDao - GetProducts: "<<query.lastError();
        return {};
    }

    QVector<std::shared_ptr<Product>> products;
    while(query.next()){

        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString barCode= query.value("barcode").toString();
        int stock = query.value("stock").toInt();
        int price = query.value("price").toInt();
        ProductType type = static_cast<ProductType>(query.value("type").toInt());

        products << std::make_shared<Product>(id, name, barCode, stock, price, type);

    }

    return products;
}
