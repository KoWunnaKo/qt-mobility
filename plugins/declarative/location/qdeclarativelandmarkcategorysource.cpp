#include "qdeclarativelandmarkcategorysource_p.h"
#include <QTimer>

#ifdef QDECLARATIVE_LANDMARK_DEBUG
#include <QDebug>
#endif

QTM_BEGIN_NAMESPACE

QDeclarativeLandmarkCategorySource::QDeclarativeLandmarkCategorySource(QObject *parent) :
        QAbstractListModel(parent), m_manager(0), m_fetchRequest(0), m_active(false)
{
    // Establish role names so that they can be queried from this model
    QHash<int, QByteArray> roleNames;
    roleNames = QAbstractItemModel::roleNames();
    roleNames.insert(NameRole, "name");
    roleNames.insert(DescriptionRole, "description");
    roleNames.insert(IconUrlRole, "iconUrl");
    setRoleNames(roleNames);
    // Instantiate default manager
    m_manager = new QLandmarkManager();
}

QDeclarativeLandmarkCategorySource::~QDeclarativeLandmarkCategorySource()
{
    delete m_manager;
    delete m_fetchRequest;
}

// When the parent is valid it means that rowCount is returning the number of children of parent.
int QDeclarativeLandmarkCategorySource::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    qDebug() << "Row count requested, returning: " <<  m_categories.count();
    return m_categories.count();
}

// Returns the stored under the given role for the item referred to by the index.
QVariant QDeclarativeLandmarkCategorySource::data(const QModelIndex &index, int role) const
{
    qDebug() << "Requested index.row(), role is: " << index.row() << role;
    QLandmarkCategory category = m_categories.value(index.row());

    switch (role) {
        case NameRole:
            return category.name();
        case IconUrlRole:
            return category.iconUrl();
    }
    return QVariant();
}

QString QDeclarativeLandmarkCategorySource::name()
{
    if (m_manager)
        return m_manager->managerName() ;
    return QString();
}

QString QDeclarativeLandmarkCategorySource::error()
{
    return m_error;
}

void QDeclarativeLandmarkCategorySource::setActive(bool active)
{
    if (active == m_active)
        return;
    if (active)
        QTimer::singleShot(0, this, SLOT(update())); // delay ensures all properties have been set
    else
        cancelUpdate();
}

bool QDeclarativeLandmarkCategorySource::isActive() const
{
    return m_active;
}

void QDeclarativeLandmarkCategorySource::update()
{
#ifdef QDECLARATIVE_LANDMARK_DEBUG
    qDebug("QDeclarativeLandmarkCategorySource::update()");
#endif
    if (!m_manager)
        return;
    // Clear any previous updates and request new
    cancelUpdate();
    m_fetchRequest = new QLandmarkCategoryFetchRequest(m_manager, this);
    QObject::connect(m_fetchRequest, SIGNAL(stateChanged(QLandmarkAbstractRequest::State)), this, SLOT(fetchRequestStateChanged(QLandmarkAbstractRequest::State)));
    // Set filters and hints. Fetch orders are not currently supported.
    m_fetchRequest->start();
    if (!m_active) {
        m_active = true;
        emit activeChanged(m_active);
    }
}

void QDeclarativeLandmarkCategorySource::cancelUpdate()
{
    if (m_fetchRequest) {
        delete m_fetchRequest;
        m_fetchRequest = 0;
    }
}

void QDeclarativeLandmarkCategorySource::convertCategoriesToDeclarative()
{
    qDebug("convertCategoriesToDeclarative enter");
    foreach(const QLandmarkCategory& category, m_categories) {
        if (!m_categoryMap.contains(category.categoryId().localId())) {
            qDebug() << "convertCategoriesToDeclarative entering new: " << category.name();
            QDeclarativeLandmarkCategory* declarativeLandmarkCategory = new QDeclarativeLandmarkCategory(this);
            declarativeLandmarkCategory->setCategory(category);
            m_categoryMap.insert(category.categoryId().localId(), declarativeLandmarkCategory);
        } else {
            // The landmark exists already, update it
            qDebug() << "convertCategoriesToDeclarative updating existing: " << category.name();
            m_categoryMap.value(category.categoryId().localId())->setCategory(category);
        }
    }
    qDebug("convertCategoriesToDeclarative exit");
}

void QDeclarativeLandmarkCategorySource::fetchRequestStateChanged(QLandmarkAbstractRequest::State state)
{
#ifdef QDECLARATIVE_LANDMARK_DEBUG
    qDebug() << "QDeclarativeLandmarkCategorySource::Fetch request finished with state: " << state;
    if (m_fetchRequest)
        qDebug() << "and related request error code is: " << m_fetchRequest->errorString();
#endif
    if (!m_fetchRequest || state != QLandmarkAbstractRequest::FinishedState)
        return;

    if (m_fetchRequest->error() == QLandmarkManager::NoError) {
        // TODO Later improvement item is to make udpate incremental by connecting to resultsAvailable() -function.
        qDebug("got no error from fetch request, updating categories in the model");
        beginInsertRows(QModelIndex(), 0, m_categories.count());
        m_categories = m_fetchRequest->categories();
        endInsertRows();
    } else if (m_error != m_fetchRequest->errorString()) {
        qDebug("got error from fetch request, updating error");
        m_error = m_fetchRequest->errorString();
        emit errorChanged(m_error);
    }
    // Convert into declarative classes --> possible to return categories in a list in QML
    convertCategoriesToDeclarative();
}

#include "moc_qdeclarativelandmarkcategorysource_p.cpp"

QTM_END_NAMESPACE