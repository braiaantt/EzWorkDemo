#include "productsmanager.h"
#include <QStringListModel>
#include <QAbstractItemView>

ProductsManager::ProductsManager()
{
    currentList = List::Filtered;
}

//------ Initialization ------

void ProductsManager::setProducts(const QVector<std::shared_ptr<Product>> &_products)
{
    products = _products;
    filteredList = _products;

    for(const std::shared_ptr<Product> &product : std::as_const(filteredList)){

        if(product->isLow()) lowStockList.append(product);

    }

    orderBy(OrderBy::Name, true);
}

//------ CRUD ------

void ProductsManager::updateProduct(int id, const QString &newName, int newStock, int newPrice)
{
    for(const std::shared_ptr<Product> &product : std::as_const(products)){

        if(product->getId() != id)
            continue;

        product->setName(newName);
        product->setPrice(newPrice);
        product->setStock(newStock);

        if(product->isLow() && !lowStockList.contains(product)){
            lowStockList.append(product);
        } else if(!product->isLow() && lowStockList.contains(product)){
            lowStockList.removeOne(product);
        }

        break;
    }
}

void ProductsManager::addProduct(std::shared_ptr<Product> newProduct)
{
    newProduct->setId(ids);
    ids++;
    products.append(newProduct);

    if(newProduct->isLow()) lowStockList.append(newProduct);
}

void ProductsManager::deleteProduct(int productId)
{
    for(int i = 0; i<products.size(); i++){

        const std::shared_ptr<Product> &product = products[i];
        if(product->getId() != productId)
            continue;

        filteredList.removeOne(product);
        lowStockList.removeOne(product);
        products.removeAt(i);

        break;
    }
}

//------ Utils ------

void ProductsManager::updateStockProducts(const QVector<CartProduct>& products)
{
    QVector<CartProduct> list = products;

    for(CartProduct &product : list){
        product.substractAmount();
        if(product.isLow() && !lowStockList.contains(product.getProduct())) lowStockList.append(product.getProduct());
    }
}

void ProductsManager::rollbackStockProducts(const QVector<CartProduct>& products)
{
    QVector<CartProduct> list = products;

    for(CartProduct &product : list){
        product.setStock(product.getStock() + product.getAmount());
    }
}

//------ Organization ------

void ProductsManager::filter(const QString &arg, OrderBy typeOrder, bool asc)
{
    const QVector<std::shared_ptr<Product>>& baseList = (currentList == List::LowStock) ? lowStockList : products;

    filteredList.clear();
    QString lowerArg = arg.toLower();

    for(const std::shared_ptr<Product>& product : baseList){

        QString name = product->getName().toLower();
        QString barcode = product->getBarCode().toLower();

        if (name.contains(lowerArg) || barcode.startsWith(lowerArg))
            filteredList.append(product);

    }

    orderBy(typeOrder, asc);
}

void ProductsManager::orderBy(OrderBy orderType, bool asc)
{
    switch(orderType){
        case OrderBy::Name:

        std::sort(filteredList.begin(), filteredList.end(), [asc](const std::shared_ptr<Product> &a, const std::shared_ptr<Product> &b) {
            return asc ? a->getName().toLower() < b->getName().toLower() : a->getName().toLower() > b->getName().toLower() ;
        });

        break;

        case OrderBy::Price:

        std::sort(filteredList.begin(), filteredList.end(), [asc](const std::shared_ptr<Product> &a, const std::shared_ptr<Product> &b) {
            return asc ? a->getPrice() < b->getPrice() : a->getPrice() > b->getPrice();
        });

        break;

        case OrderBy::Stock:

        std::sort(filteredList.begin(), filteredList.end(), [asc](const std::shared_ptr<Product> &a, const std::shared_ptr<Product> &b) {
            return asc ? a->getStock() < b->getStock() : a->getStock() > b->getStock();
        });

        break;
    }
}

void ProductsManager::reverse()
{
    if(!filteredList.isEmpty()) std::reverse(filteredList.begin(), filteredList.end());
}

//------ Getters ------

std::shared_ptr<Product> ProductsManager::getProduct(const QString &input) const
{
    for(const std::shared_ptr<Product> &product : std::as_const(products)){

        if(product->getName() == input || product->getBarCode() == input)
            return product;
    }

    return nullptr;
}

std::shared_ptr<Product> ProductsManager::getProduct(int productId) const
{
    for(const std::shared_ptr<Product> &product : std::as_const(products)){

        if(product->getId() == productId)
            return product;
    }

    return nullptr;
}

QVector<std::shared_ptr<Product>> ProductsManager::getProductsSince(int pos, int max) const
{
    QVector<std::shared_ptr<Product>> list;

    for(int i = pos; i<filteredList.size(); i++){

        if(max == 0) break;

        list.append(filteredList[i]);
        max--;

    }
    return list;
}

QCompleter* ProductsManager::getCompleter(QObject *parent) const
{
    QStringList list;

    for(const std::shared_ptr<Product> &product : std::as_const(products)){
        list.append(product->getName());
    }

    QCompleter *completer = new QCompleter(list, parent);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->popup()->setFont(QFont("Arial", 16));

    return completer;
}

const QVector<std::shared_ptr<Product>>& ProductsManager::getProducts() const
{
    return products;
}

const QVector<std::shared_ptr<Product>>& ProductsManager::getFilteredList() const
{
    return filteredList;
}

const QVector<std::shared_ptr<Product>>& ProductsManager::getLowStockList() const
{
    return lowStockList;
}

//------ Setters ------

void ProductsManager::setCurrentList(List typeList)
{
    currentList = typeList;
}
