#pragma once
#include <utility>
#include <optional>

namespace ClosestGL::Primitive
{
	/* CVV������ 
	 *
	 * ����ͼԪ�Ķ�����ʹ��
	 * ��ͼԪ�Ķ���������Ϲ��˳�ͨ�����õ�ͼԪ��
	 * ģ�������
	 *        TPrimitiveReader - ԭʼ���͵�ͼԪ�Ķ���
	 *        TVertex          - ������������
	 * ���캯������˵����
	 *     ���캯������Ĳ���Ϊ���㻺���TPrimitiveReader�Ĺ��캯��������
	 */
	template<typename TPrimitiveReader,typename TVertex>
	class CVVClipper
	{
	private:
		TPrimitiveReader* const reader_;
		const TVertex* const vb_;

	public:
		static constexpr auto GetVertexPerPrimitive() { return TPrimitiveReader::GetVertexPerPrimitive(); }
		using Primitive = decltype(reader_->Read());
		
	private:
		std::optional<Primitive> next_;

		void PrepareNextPrimitive()
		{
			next_.reset();

			while (reader_->CanRead())
			{
				auto primitive = reader_->Read();

				int clipped = 0;
				for (int i = 0; i < GetVertexPerPrimitive(); ++i)
				{
					const auto& v = vb_[primitive[i]].SVPosition;
					auto w = v.w;

					if (v.z < 0 || v.z >  w || v.x < -w || v.x >  w || v.y < -w || v.y >  w)
					{
						clipped++;
					}

					if (w <= 0)
					{
						clipped = GetVertexPerPrimitive();
						break;
					}
				}

				if (clipped < GetVertexPerPrimitive())
				{
					next_ = std::move(primitive);
					break;
				}
			}
		}

	public:
		CVVClipper(const TVertex* vertexBuffer, TPrimitiveReader* reader):
			reader_ { reader },
			vb_{ vertexBuffer }
		{
			PrepareNextPrimitive();
		}

		

		inline void Reset()
		{
			reader_->Reset();
			PrepareNextPrimitive();
		}

		inline auto Read()
		{
			auto ret = std::move(*next_);
			PrepareNextPrimitive();
			return ret;
		}

		inline bool CanRead()
		{
			return next_.has_value();
		}

		void WriteToVertexList(std::vector<Primitive>& output)
		{
			while (CanRead())
			{
				output.push_back(Read());
			}
		}
	};
}