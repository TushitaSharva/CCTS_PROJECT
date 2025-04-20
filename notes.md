Apr 20th, 2025, 15:26

1. Installed cmake system wide
2. Download gtest in `lib` dir
3. Added condition variable to each node
4. Returning the node created to the transaction so that the transaction can wait on the condition variable included in the returned node.
5. Added an overall lock to the nodeList `listMutex`.
6. Waiting transaction syntax:

   ```
   Node* node = list->addReadNode(t);
   std::unique_lock<std::mutex> lock(node->mtx);
   node->cv.wait(lock, [&]() {return node->isAtHead; });
   ```
7. Added make file - run tests using the command:  `make test`
