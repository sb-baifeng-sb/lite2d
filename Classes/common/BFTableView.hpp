//
//  BFTableView.hpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/8.
//

#ifndef BFTableView_hpp
#define BFTableView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "app/macros.h"

NS_BF_BEGIN

class BFTableView : public cocos2d::extension::TableView, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate {
public:
    typedef cocos2d::extension::TableView TableView;
    typedef cocos2d::extension::TableViewDataSource DataSource;
    typedef cocos2d::extension::TableViewDelegate Delegate;
    typedef cocos2d::extension::TableViewCell TableViewCell;
public:
    typedef std::function<cocos2d::Size(TableView* table, ssize_t idx)> CellSizeFunc;
    typedef std::function<TableViewCell*(TableView* table, ssize_t idx)> TableCellFunc;
    typedef std::function<ssize_t(TableView* table)> NumberOfCellsFunc;
    typedef std::function<void(TableView* table, TableViewCell* cell)> CellTouchedFunc;
public:
    CREATE_FUNC(BFTableView);
    BFTableView();
public:
    void setViewSize(cocos2d::Size const& size); // must call
public:
    // dataSource
    virtual cocos2d::Size cellSizeForTable(TableView* table); // no use
    virtual cocos2d::Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
public:
    // delete
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
public:
    CellSizeFunc TableCellSizeForIndex;
    TableCellFunc TableCellAtIndex;
    NumberOfCellsFunc NumberOfCellsInTableView;
    CellTouchedFunc TableCellTouched;
};

NS_BF_END

#endif /* BFTableView_hpp */
