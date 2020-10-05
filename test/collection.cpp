#include <iostream>

#include <ext/shared_recursive_mutex>
#include <ext/collection>
#include <gtest/gtest.h>

class data {
public:
  data() : id(0), value(0) {}
  int id;
  int value;
};

class managed_data : public ext::collection<managed_data>::item {
public:
  managed_data() : id(0), value(0) {}
  int id;
  int value;
};

using data_list_rw = ext::collection<data>;
using data_list_r = ext::const_collection<data>;

using managed_data_list_rw = ext::collection<managed_data>;
using managed_data_list_r = ext::const_collection<managed_data>;

managed_data global_managed_data_list[20];
const size_t global_managed_data_list_count =
    sizeof(global_managed_data_list) / sizeof(managed_data);

TEST(collection_test, manual_add) {
  ext::collection_mgr<data> mgr;

  size_t count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 0);

  // Manual add
  data *item = new data;
  item->id = 1;
  item->value = 10;
  mgr.add(item);

  count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 1);
}

TEST(collection_test, manual_remove) {
  ext::collection_mgr<data> mgr;

  size_t count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 1);

  // Manual add
  data *item = new data;
  item->id = 2;
  item->value = 20;
  mgr.add(item);

  count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 2);

  // Manual remove (Invalid operation)
  mgr.remove(nullptr);
  count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 2);

  // Manual remove
  mgr.remove(item);
  count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 1);

  // Manual remove (Remove added item from manual_add test routine)
  std::list<data *> list;
  for (auto item : data_list_rw())
    list.push_back(item);
  for (auto item : list) {
    mgr.remove(item);
    delete item;
  }

  count = 0;
  for (auto item : data_list_r())
    count++;
  EXPECT_EQ(count, 0);
}

TEST(collection_test, auto_add) {
  // Auto add
  const size_t data_count = 20;
  managed_data items[data_count];
  size_t count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count);

  // Auto add
  std::shared_ptr<managed_data> ptr1 = std::make_shared<managed_data>();
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 1);

  // Auto add
  managed_data *ptr2 = new managed_data;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 2);

  // Auto add
  managed_data var1;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 3);

  // Manual remove
  ext::collection_mgr<managed_data> mgr;
  mgr.remove(&items[0]);
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 3 - 1);

  // Manual remove
  mgr.remove(&items[1]);
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 3 - 2);

  // Auto remove
  delete ptr2;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 3 - 3);

  // Auto remove
  ptr1 = nullptr;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 3 - 4);
}

TEST(collection_test, auto_remove) {
  const size_t data_count = 20;
  managed_data items[data_count];

  // Auto add
  std::shared_ptr<managed_data> ptr1 = std::make_shared<managed_data>();
  managed_data *ptr2 = new managed_data;
  size_t count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 2);

  // Auto remove
  ptr1 = nullptr;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count + 1);

  // Auto remove
  delete ptr2;
  count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count + data_count);
}

TEST(collection_test, auto_add_and_set_value) {
  // Auto Add
  std::shared_ptr<managed_data> ptr1 = std::make_shared<managed_data>();
  ptr1->id = 1;
  ptr1->value = 10;

  // Auto Add
  managed_data item1;
  item1.id = 2;
  item1.value = 20;

  for (auto item : managed_data_list_rw()) {
    EXPECT_EQ(typeid(managed_data *), typeid(item));

    if (item->id == 1) {
      EXPECT_EQ(item->value, 10);
      item->value = 20;
    }

    if (item->id == 2) {
      EXPECT_EQ(item->value, 20);
      item->value = 40;
    }
  }

  for (auto item : managed_data_list_r()) {
    EXPECT_EQ(typeid(const managed_data *), typeid(item));

    if (item->id == 1) {
      EXPECT_EQ(item->value, 20);
    }

    if (item->id == 2) {
      EXPECT_EQ(item->value, 40);
    }
  }
}

TEST(collection_test, no_add) {
  data items[100];
  (void)items;

  size_t count = 0;
  for (auto item : managed_data_list_r())
    count++;
  EXPECT_EQ(count, global_managed_data_list_count);
}
