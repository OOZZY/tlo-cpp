#include <tlo-cpp/sqlite3.hpp>
#include <tlo-cpp/test.hpp>

namespace {
TLO_TEST(Sqlite3ConnectionDefaultConstructor) {
  tlo::Sqlite3Connection connection;

  TLO_EXPECT_FALSE(connection.isOpen());
}

TLO_TEST(Sqlite3StatementDefaultConstructor) {
  tlo::Sqlite3Statement statement;

  TLO_EXPECT_FALSE(statement.isPrepared());
}

TLO_TEST(Sqlite3OpenConnectionAndPrepareStatements) {
  tlo::Sqlite3Connection connection(":memory:");

  TLO_ASSERT(connection.isOpen());

  tlo::Sqlite3Statement create(connection,
                               "CREATE TABLE ErrorCode (value INTEGER PRIMARY "
                               "KEY NOT NULL, desc TEXT NOT NULL);");

  TLO_ASSERT(create.isPrepared());

  auto rc = create.step();

  TLO_ASSERT_EQ(rc, SQLITE_DONE);

  tlo::Sqlite3Statement insert(connection,
                               "INSERT INTO ErrorCode VALUES(:value, :desc);");
  tlo::Sqlite3Statement select(connection, "SELECT * FROM ErrorCode;");
  tlo::Sqlite3Statement update(
      connection, "UPDATE ErrorCode SET desc = :desc WHERE value = :value;");
  tlo::Sqlite3Statement remove(connection,
                               "DELETE FROM ErrorCode WHERE value = :value;");

  TLO_ASSERT(insert.isPrepared());
  TLO_ASSERT(select.isPrepared());
  TLO_ASSERT(update.isPrepared());
  TLO_ASSERT(remove.isPrepared());

  insert.bindInt(":value", 0);
  insert.bindUtf8Text(":desc", "Zero means success.");
  rc = insert.step();
  TLO_ASSERT_EQ(rc, SQLITE_DONE);
  insert.reset();

  try {
    insert.step();
    TLO_EXPECT(false && "Inserting a duplicate should fail.");
  } catch (...) {
  }

  int numRows = 0;

  while (select.step() != SQLITE_DONE) {
    int value = select.columnAsInt(0);
    std::string desc = select.columnAsUtf8Text(1).data();

    TLO_EXPECT_EQ(value, 0);
    TLO_EXPECT_EQ(desc, "Zero means success.");
    numRows++;
  }

  TLO_ASSERT_EQ(numRows, 1);

  update.bindInt(":value", 0);
  update.bindUtf8Text(":desc", "Zero means no errors occurred.");
  rc = update.step();
  TLO_ASSERT_EQ(rc, SQLITE_DONE);

  select.reset();
  numRows = 0;

  while (select.step() != SQLITE_DONE) {
    int value = select.columnAsInt(0);
    std::string desc = select.columnAsUtf8Text(1).data();

    TLO_EXPECT_EQ(value, 0);
    TLO_EXPECT_EQ(desc, "Zero means no errors occurred.");
    numRows++;
  }

  TLO_ASSERT_EQ(numRows, 1);

  remove.bindInt(":value", 0);
  rc = remove.step();
  TLO_ASSERT_EQ(rc, SQLITE_DONE);

  select.reset();
  numRows = 0;

  while (select.step() != SQLITE_DONE) {
    numRows++;
  }

  TLO_ASSERT_EQ(numRows, 0);
}
}  // namespace
