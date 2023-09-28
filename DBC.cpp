#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class DBConnection {
public:
	DBConnection() {}
	virtual ~DBConnection() {}

	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual int execQuery(int query) = 0;
};

class ClassThatUsesDB {
public:
	ClassThatUsesDB()  {}
	virtual ~ClassThatUsesDB() {}
	
	void openConnection(DBConnection * connection_on) {
		connection_on->open();
	};
	void closeConnection(DBConnection* DB) {
		if (DB->open() == true) {
			DB->close();
		}
	};
	int useConnection(DBConnection* DB, int query) {
		if (DB->open() == true) {
			int res = DB->execQuery(query);
			DB->close();
			return res;
		}
	};
};

class MockDB : public DBConnection {
public:
	MOCK_METHOD(bool, open, (), (override));
	MOCK_METHOD(bool, close, (), (override));
	MOCK_METHOD(int, execQuery, (int query), (override));
};

class TestSuite : public ::testing::Test {
protected:
	void SetUp() {
		Base = new ClassThatUsesDB();
	}

	void TearDown() {
		delete Base;
	}

	ClassThatUsesDB* Base;
};

TEST_F(TestSuite, testcase1) {
	
	MockDB mock_conect;
	int query = 13;
	
	EXPECT_CALL(mock_conect, open).WillOnce(::testing::Return(true));
	EXPECT_CALL(mock_conect, close).WillOnce(::testing::Return(true));

	EXPECT_CALL(mock_conect, execQuery(query)).WillOnce(::testing::Return(13));

	int responce = Base->useConnection(&mock_conect, query);

	ASSERT_EQ(query, responce);

}


int main(int argc, char* argv[]) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}