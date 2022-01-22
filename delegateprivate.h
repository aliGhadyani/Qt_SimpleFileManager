#ifndef DELEGATEPRIVATE_H
#define DELEGATEPRIVATE_H

#include <qglobal.h>
#include <QStyleOptionViewItem>


class DelegatePrivate
{
  DelegatePrivate();

  inline QRect timestampBox(const QStyleOptionViewItem &option, const QModelIndex &index) const;
  inline qreal timestampFontPointSize(const QFont &f) const;
  inline QRect messageBox(const QStyleOptionViewItem &option) const;

  QSize iconSize;
  QMargins margins;
  int spacingHorizontal;
  int spacingVertical;
  friend class TileViewItem;
};

#endif // DELEGATEPRIVATE_H

