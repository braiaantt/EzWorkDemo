#include "productservice.h"
#include <QSqlQuery>
#include <QSqlError>

ProductService::ProductService(QSqlDatabase &db, ProductDao &productDao, LogDao &logDao) :
    db(db), productDao(productDao), logDao(logDao)
{

}

bool ProductService::updateProduct(int productId, const QString &newName, int newStock, int newPrice, const QVector<LogData> &logs) const
{
    if(!db.transaction()){
        qWarning()<<"ProductService: Transaction Error - "<<db.lastError();
        return false;
    }

    if(!productDao.updateProduct(productId, newName, newStock, newPrice)){
        db.rollback();
        return false;
    }

    if(!logDao.addLog(logs)){
        db.rollback();
        return false;
    }

    if(!db.commit()){
        db.rollback();
        qWarning()<<"ProductService: Commit Error - "<<db.lastError();
        return false;
    }

    return true;
}

//------ Getters ------

ProductDao* ProductService::getProductDao() const
{
    return &productDao;
}
