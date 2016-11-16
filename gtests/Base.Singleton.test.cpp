#include <gtest/gtest.h>

#include <Base/Singleton.h>
namespace
{
	class SingletonTest
	{
	public:
		SingletonTest()
			: m_value(0)
		{
			m_construct++;
		}

		~SingletonTest()
		{
			m_construct--;
		}

		void Add()
		{
			m_value++;
		}

		int			m_value;
		static int	m_construct;
	};

	int SingletonTest::m_construct = 0;
}

TEST(BaseTest, Singleton)
{
	auto inst = saba::Singleton<SingletonTest>::Get();

	EXPECT_NE(nullptr, inst);

	// �R���X�g���N�^���Ă΂�Ă��邱�Ƃ��m�F
	EXPECT_EQ(0, saba::Singleton<SingletonTest>::Get()->m_value);

	// ����̃C���X�^���X��Add���Ă΂�Ă��邱�Ƃ��m�F
	saba::Singleton<SingletonTest>::Get()->Add();
	EXPECT_EQ(1, saba::Singleton<SingletonTest>::Get()->m_value);

	// �R���X�g���N�^�[����x�����Ă΂�Ă��Ȃ����Ƃ��m�F
	EXPECT_EQ(1, SingletonTest::m_construct);

	saba::SingletonFinalizer::Finalize();
	saba::SingletonFinalizer::Finalize();

	// �f�X�g���N�^�[���Ă΂�Ă��邱�Ƃ��m�F
	EXPECT_EQ(0, SingletonTest::m_construct);
}
