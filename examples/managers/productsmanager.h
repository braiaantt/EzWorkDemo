#ifndef PRODUCTSMANAGER_H
#define PRODUCTSMANAGER_H

#include <QVector>
#include <QCompleter>
#include "product.h"
#include "cartproduct.h"

enum class OrderBy{
    Name = 0,
    Price = 1,
    Stock = 2
};

enum class List{
    Filtered = 0,
    LowStock = 1
};

class ProductsManager
{
public:
    ProductsManager();

    //------ Initialization ------

    void setProducts(const QVector<std::shared_ptr<Product>> &products);

    //------ CRUD ------
    void addProduct(std::shared_ptr<Product> newProduct);
    void updateProduct(int id, const QString &newName, int newStock, int newPrice);
    void deleteProduct(int productId);

    //------ Utils ------
    void updateStockProducts(const QVector<CartProduct>& products);
    void rollbackStockProducts(const QVector<CartProduct> &products);

    //------ Organization ------

    void filter(const QString &arg, OrderBy orderType, bool asc);
    void orderBy(OrderBy orderType, bool asc);
    void reverse();

    //------ Getters ------
    std::shared_ptr<Product> getProduct(const QString &input) const;
    std::shared_ptr<Product> getProduct(int productId) const;
    QVector<std::shared_ptr<Product>> getProductsSince(int pos, int max) const;
    QCompleter *getCompleter(QObject *parent) const;

    const QVector<std::shared_ptr<Product>>& getProducts() const;
    const QVector<std::shared_ptr<Product>>& getFilteredList() const;
    const QVector<std::shared_ptr<Product>>& getLowStockList() const;

    //------ Setters ------
    void setCurrentList(List typeList);

private:
    QVector<std::shared_ptr<Product>> products;
    QVector<std::shared_ptr<Product>> lowStockList;
    QVector<std::shared_ptr<Product>> filteredList;
    List currentList;
};

#endif // PRODUCTSMANAGER_H
