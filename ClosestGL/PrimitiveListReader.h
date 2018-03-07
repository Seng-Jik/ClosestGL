#pragma once
#include <array>

namespace ClosestGL::Primitive
{

	/* ͼԪ�б��Ķ��� */

	/* ͼԪ�б��Ķ���������List����ʽ��ȡͼԪ */

	/* ����ͼԪ�Ķ��� 
	 * ͼԪ�Ķ�����ģ�����VertexPerPrimitive��ָʾÿ���ٸ����㹹��һ��ͼԪ��
	 * ͼԪ�Ķ������ڴ�IndexBufferȡ��ͼԪ��
	 * �����б�ʽ��IndexBuffer����Ҫʹ��ͼԪ�б��Ķ�����
	 * ����Stripʽ��IndexBuffer������Ҫʹ��ͼԪStrip�Ķ�����
	 * ͼԪ�Ķ������������IndexBuffer��ȡ������ͼԪ�е����ж��㡣
	 * ͼԪ�Ķ����Ĺ��캯������indexBuffer�����Ĵ�С���Թ���һ��ͼԪ�Ķ�����
	 * ʹ��Reset��������״̬�Դ�ͷ���¶�ȡͼԪ��
	 * CanRead����������ѯ�Ƿ���Լ����Ķ�ͼԪ��
	 * Read�������ڶ�ȡһ��ͼԪ���������ReadʱCanRead����ֵΪfalse����δ���塣
	 */

	template<int VertexPerPrimitive>
	class PrimitiveListReader
	{
	private:
		size_t* indexBuffer_;
		size_t size_;
		size_t offset_;
	public:
		static constexpr int GetVertexPerPrimitive() { return VertexPerPrimitive; }

		inline PrimitiveListReader(size_t* indexBuffer, size_t size):
			indexBuffer_{ indexBuffer },
			size_{ size },
			offset_{0}
		{}

		PrimitiveListReader(const PrimitiveListReader&) = delete;

		inline void Reset()
		{
			offset_ = 0;
		}

		inline std::array<size_t,VertexPerPrimitive> Read()
		{
			std::array<size_t,VertexPerPrimitive> ret;

			for (int i = 0; i < VertexPerPrimitive; ++i)
				ret[i] = indexBuffer_[offset_++];

			return ret;
		}

		inline bool CanRead()
		{
			return offset_ < size_ - (VertexPerPrimitive - 1);
		}
	};
}
