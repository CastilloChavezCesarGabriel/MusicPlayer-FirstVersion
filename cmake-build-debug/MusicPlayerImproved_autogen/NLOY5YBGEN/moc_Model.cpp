/****************************************************************************
** Meta object code from reading C++ file 'Model.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../model/Model.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN5ModelE_t {};
} // unnamed namespace

template <> constexpr inline auto Model::qt_create_metaobjectdata<qt_meta_tag_ZN5ModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Model",
        "songs_updated",
        "",
        "songs",
        "ads_updated",
        "ads",
        "disable_buttons"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'songs_updated'
        QtMocHelpers::SignalData<void(const QStringList &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 3 },
        }}),
        // Signal 'ads_updated'
        QtMocHelpers::SignalData<void(const QStringList &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 5 },
        }}),
        // Signal 'disable_buttons'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Model, qt_meta_tag_ZN5ModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Model::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5ModelE_t>.metaTypes,
    nullptr
} };

void Model::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Model *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->songs_updated((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 1: _t->ads_updated((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 2: _t->disable_buttons(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Model::*)(const QStringList & )>(_a, &Model::songs_updated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Model::*)(const QStringList & )>(_a, &Model::ads_updated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Model::*)()>(_a, &Model::disable_buttons, 2))
            return;
    }
}

const QMetaObject *Model::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Model::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5ModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Model::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Model::songs_updated(const QStringList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Model::ads_updated(const QStringList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Model::disable_buttons()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
