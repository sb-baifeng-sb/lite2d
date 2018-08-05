//
//  BFTableView.cpp
//  HelloTuiCpp
//
//  Created by baifeng on 2018/6/8.
//

#include "BFTableView.hpp"

NS_BF_BEGIN

BFTableView::BFTableView() {
    this->setDataSource(this);
    this->setDelegate(this);
}

void BFTableView::setViewSize(cocos2d::Size const& size) {
    this->initWithViewSize(size);
}

cocos2d::Size BFTableView::cellSizeForTable(TableView* table) {
    return cocos2d::Size::ZERO;
}

cocos2d::Size BFTableView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    if (TableCellSizeForIndex != nullptr) {
        return TableCellSizeForIndex(table, idx);
    }
    return cellSizeForTable(table);
}

BFTableView::TableViewCell* BFTableView::tableCellAtIndex(TableView *table, ssize_t idx) {
    if (TableCellAtIndex != nullptr) {
        return TableCellAtIndex(table, idx);
    }
    return nullptr;
}

ssize_t BFTableView::numberOfCellsInTableView(TableView *table) {
    if (NumberOfCellsInTableView != nullptr) {
        return NumberOfCellsInTableView(table);
    }
    return 0;
}

void BFTableView::tableCellTouched(TableView* table, TableViewCell* cell) {
    if (TableCellTouched != nullptr) {
        TableCellTouched(table, cell);
    }
}

void BFTableView::tableCellHighlight(TableView* table, TableViewCell* cell) {
    TableViewDelegate::tableCellHighlight(table, cell);
}

void BFTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    TableViewDelegate::tableCellUnhighlight(table, cell);
}

void BFTableView::tableCellWillRecycle(TableView* table, TableViewCell* cell) {
    TableViewDelegate::tableCellWillRecycle(table, cell);
}

NS_BF_END
