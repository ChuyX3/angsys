#pragma once
#include <media\file_streamer.h>

namespace ang
{
	namespace media
	{	
		class media_streamer
		{
		private:
			WAVEFORMATEX _wave_format;
			uint _max_stream_length;

		public:
			file_streamer_t _file;
			com_wrapper<IMFSourceReader> _reader;
			com_wrapper<IMFMediaType> _audio_type;

		public:
			media_streamer();
			~media_streamer();

			WAVEFORMATEX& output_wave_format() { return _wave_format; }
			WAVEFORMATEX const& output_wave_format()const { return _wave_format; }
			uint max_stream_length()const { return _max_stream_length; }

			void init(cwstr_t url);
			bool next_buffer(const_array<byte> buffer, uint& length);
			void read_all(const_array<byte> buffer, uint& length);
			void restart();
		};
	}
}