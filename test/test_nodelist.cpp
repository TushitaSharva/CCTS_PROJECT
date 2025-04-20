#include <gtest/gtest.h>
#include "../NodeList.h"
#include "../Transaction.h"

class NodeListTest : public ::testing::Test {
protected:
    NodeList list;
};

TEST_F(NodeListTest, InitNodeList) {
    ASSERT_EQ(list.head, nullptr);
    ASSERT_EQ(list.tail, nullptr);
}

TEST_F(NodeListTest, AddReadNodeToEmptyList) {
    Transaction* t = new Transaction(1, 1);
    Node* node = list.addReadNode(t);

    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->type, READ);
    ReadNode* rnode = dynamic_cast<ReadNode*>(node);
    ASSERT_NE(rnode, nullptr);
    EXPECT_EQ(rnode->tlist.size(), 1);
    EXPECT_TRUE(rnode->tlist.count(t));
    EXPECT_EQ(rnode->isAtHead, true);

    ASSERT_EQ(list.head, rnode);
    ASSERT_EQ(list.tail, rnode);
    ASSERT_EQ(list.tail->next, nullptr);
}

TEST_F(NodeListTest, AddTwoReadTransactionsToSameReadNode) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node1 = list.addReadNode(t1);
    Node* node2 = list.addReadNode(t2);

    EXPECT_EQ(node1, node2);
    ReadNode* rnode = dynamic_cast<ReadNode*>(node1);
    ASSERT_NE(rnode, nullptr);
    EXPECT_EQ(rnode->tlist.size(), 2);
    EXPECT_TRUE(rnode->tlist.count(t1));
    EXPECT_TRUE(rnode->tlist.count(t2));
    EXPECT_EQ(rnode->isAtHead, true);

    ASSERT_EQ(list.head, rnode);
    ASSERT_EQ(list.tail, rnode);
    ASSERT_EQ(list.tail->next, nullptr);
}

TEST_F(NodeListTest, AddWriteNodeToEmptyList) {
    Transaction* t = new Transaction(1, 1);
    Node* node = list.addWriteNode(t);

    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->type, WRITE);
    WriteNode* wnode = dynamic_cast<WriteNode*>(node);
    ASSERT_NE(wnode, nullptr);
    EXPECT_EQ(wnode->transaction, t);
    EXPECT_EQ(wnode->isAtHead, true);

    ASSERT_EQ(list.head, wnode);
    ASSERT_EQ(list.tail, wnode);
    ASSERT_EQ(list.tail->next, nullptr);
}

TEST_F(NodeListTest, AddTwoWriteTransactionsToDifferentNodes) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node1 = list.addWriteNode(t1);
    Node* node2 = list.addWriteNode(t2);

    EXPECT_NE(node1, nullptr);
    EXPECT_NE(node2, nullptr);
    EXPECT_NE(node1, node2);
    WriteNode* wnode1 = dynamic_cast<WriteNode*>(node1);
    WriteNode* wnode2 = dynamic_cast<WriteNode*>(node2);
    ASSERT_NE(wnode1, nullptr);
    EXPECT_EQ(wnode1->transaction, t1);
    EXPECT_EQ(wnode1->isAtHead, true);
    ASSERT_NE(wnode2, nullptr);
    EXPECT_EQ(wnode2->transaction, t2);
    EXPECT_EQ(wnode2->isAtHead, false);

    ASSERT_EQ(list.head, wnode1);
    ASSERT_EQ(list.tail, wnode2);
    ASSERT_EQ(list.head->next, wnode2);
}

TEST_F(NodeListTest, AddWriteAndTwoReadTransactions) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Transaction* t3 = new Transaction(3, 3);
    Node* node1 = list.addWriteNode(t1);
    Node* node2 = list.addReadNode(t2);
    Node* node3 = list.addReadNode(t3);

    EXPECT_NE(node1, nullptr);
    EXPECT_NE(node2, nullptr);
    EXPECT_NE(node3, nullptr);
    WriteNode* wnode1 = dynamic_cast<WriteNode*>(node1);
    ReadNode* rnode2 = dynamic_cast<ReadNode*>(node2);
    ReadNode* rnode3 = dynamic_cast<ReadNode*>(node3);

    ASSERT_NE(wnode1, nullptr);
    ASSERT_NE(rnode2, nullptr);
    ASSERT_NE(rnode3, nullptr);
    EXPECT_EQ(rnode2, rnode3);
    EXPECT_EQ(wnode1->transaction, t1);
    EXPECT_EQ(wnode1->isAtHead, true);
    EXPECT_EQ(rnode2->tlist.size(), 2);
    EXPECT_TRUE(rnode2->tlist.count(t2));
    EXPECT_TRUE(rnode2->tlist.count(t3));
    EXPECT_EQ(rnode2->isAtHead, false);

    ASSERT_EQ(list.head, wnode1);
    ASSERT_EQ(list.tail, rnode2);
    ASSERT_EQ(list.head->next, rnode2);
}

TEST_F(NodeListTest, AddReadWriteReadTransactions) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Transaction* t3 = new Transaction(3, 3);
    Node* node1 = list.addReadNode(t1);
    Node* node2 = list.addWriteNode(t2);
    Node* node3 = list.addReadNode(t3);

    EXPECT_NE(node1, nullptr);
    EXPECT_NE(node2, nullptr);
    EXPECT_NE(node3, nullptr);
    ReadNode* rnode1 = dynamic_cast<ReadNode*>(node1);
    WriteNode* wnode2 = dynamic_cast<WriteNode*>(node2);
    ReadNode* rnode3 = dynamic_cast<ReadNode*>(node3);

    ASSERT_NE(rnode1, nullptr);
    ASSERT_NE(wnode2, nullptr);
    ASSERT_NE(rnode3, nullptr);
    EXPECT_NE(rnode1, rnode3);
    EXPECT_EQ(rnode1->tlist.size(), 1);
    EXPECT_TRUE(rnode1->tlist.count(t1));
    EXPECT_EQ(rnode1->isAtHead, true);
    EXPECT_EQ(wnode2->transaction, t2);
    EXPECT_EQ(wnode2->isAtHead, false);
    EXPECT_EQ(rnode3->tlist.size(), 1);
    EXPECT_TRUE(rnode3->tlist.count(t3));
    EXPECT_EQ(rnode3->isAtHead, false);

    ASSERT_EQ(list.head, rnode1);
    ASSERT_EQ(list.tail, rnode3);
    ASSERT_EQ(list.head->next, wnode2);
    ASSERT_EQ(list.head->next->next, rnode3);
}

TEST_F(NodeListTest, ReadNodeDeletionRemovesTransaction) {
    Transaction* t = new Transaction(1, 1);
    Node* node = list.addReadNode(t);
    list.deleteReadNode(t);

    ASSERT_EQ(list.head, nullptr);
    ASSERT_EQ(list.tail, nullptr);

    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node1 = list.addReadNode(t1);
    Node* node2 = list.addReadNode(t2);
    list.deleteReadNode(t1);
    ReadNode* rnode = dynamic_cast<ReadNode*>(node1);

    ASSERT_NE(rnode, nullptr);
    EXPECT_EQ(rnode->tlist.size(), 1);
    EXPECT_TRUE(rnode->tlist.count(t2));
    EXPECT_FALSE(rnode->tlist.count(t1));
    ASSERT_EQ(list.head, rnode);
    ASSERT_EQ(list.tail, rnode);
    ASSERT_EQ(list.head->next, nullptr);
}

TEST_F(NodeListTest, WriteNodeDeletionRemovesTransaction) {
    Transaction* t = new Transaction(1, 1);
    Node* node = list.addWriteNode(t);
    list.deleteWriteNode(t);

    ASSERT_EQ(list.head, nullptr);
    ASSERT_EQ(list.tail, nullptr);    
}

TEST_F(NodeListTest, DeleteWithMismatchType) {
    Transaction* t = new Transaction(1, 1);
    Node* node = list.addWriteNode(t);
    list.deleteReadNode(t);

    ASSERT_EQ(list.head, node);
    ASSERT_EQ(list.tail, node);

    list.deleteWriteNode(t);
    Transaction* t1 = new Transaction(1, 1);
    Node* node1 = list.addReadNode(t1);
    list.deleteWriteNode(t1);

    ASSERT_EQ(list.head, node1);
    ASSERT_EQ(list.tail, node1);
}

TEST_F(NodeListTest, DeleteOnlyWhenInHead) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node = list.addReadNode(t1);
    list.deleteReadNode(t2);
    EXPECT_EQ(list.head, node);
    EXPECT_EQ(list.tail, node);

    list.deleteReadNode(t1);
    Transaction* t3 = new Transaction(1, 1);
    Transaction* t4 = new Transaction(2, 2);
    Node* node1 = list.addWriteNode(t3);
    list.deleteReadNode(t4);
    EXPECT_EQ(list.head, node1);
    EXPECT_EQ(list.tail, node1);
}

TEST_F(NodeListTest, ReadDeletionMovesHead) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node1 = list.addReadNode(t1);
    Node* node2 = list.addWriteNode(t2);
    list.deleteReadNode(t1);

    EXPECT_EQ(node2->isAtHead, true);
    ASSERT_EQ(list.head, node2);
    ASSERT_EQ(list.tail, node2);
}

TEST_F(NodeListTest, WriteDeletionMovesHead) {
    Transaction* t1 = new Transaction(1, 1);
    Transaction* t2 = new Transaction(2, 2);
    Node* node1 = list.addWriteNode(t1);
    Node* node2 = list.addReadNode(t2);
    list.deleteWriteNode(t1);

    EXPECT_EQ(node2->isAtHead, true);
    ASSERT_EQ(list.head, node2);
    ASSERT_EQ(list.tail, node2);
}

