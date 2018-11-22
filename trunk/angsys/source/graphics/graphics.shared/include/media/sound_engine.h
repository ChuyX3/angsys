#pragma once

#include <ang/media/media.h>
#include <ang/com_wrapper.h>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mfmediaengine.h>


namespace ang
{
	namespace media
	{
		struct StreamingVoiceContext : public IXAudio2VoiceCallback
		{
			virtual void STDCALL OnVoiceProcessingPassStart(UINT32) override {}
			virtual void STDCALL OnVoiceProcessingPassEnd() override {}
			virtual void STDCALL OnStreamEnd() override {}
			virtual void STDCALL OnBufferStart(void*) override
			{
				ResetEvent(hBufferEndEvent);
			}
			virtual void STDCALL OnBufferEnd(void* pContext) override
			{
				// Trigger the event for the music stream.
				if (pContext == 0) {
					SetEvent(hBufferEndEvent);
				}
			}
			virtual void STDCALL OnLoopEnd(void*) override {}
			virtual void STDCALL OnVoiceError(void*, HRESULT) override {}

			HANDLE hBufferEndEvent;
			StreamingVoiceContext() : hBufferEndEvent(CreateEventEx(NULL, FALSE, FALSE, NULL))
			{
			}
			virtual ~StreamingVoiceContext()
			{
				CloseHandle(hBufferEndEvent);
			}
		};

		class media_streamer
		{
		private:
			WAVEFORMATEX    _wave_format;
			uint          _max_stream_length_in_bytes;

		public:
			ang::com_wrapper<IMFSourceReader> _reader;
			ang::com_wrapper<IMFMediaType> _audio_type;

		public:
			media_streamer();
			~media_streamer();

			WAVEFORMATEX& output_wave_format() { return _wave_format; }
			WAVEFORMATEX const& output_wave_format()const { return _wave_format; }
			uint max_stream_length_in_bytes()const { return _max_stream_length_in_bytes; }

			void init(cwstr_t url);
			bool get_next_buffer(const_array<byte> buffer, uint& bufferLength);
			void read_all(const_array<byte> buffer, uint& bufferLength);
			void restart();
		};



		class sound_engine;
		class sound_element;
		typedef object_wrapper<sound_engine> sound_engine_t;
		typedef object_wrapper<sound_element> sound_element_t;

		class sound_element 
			: public object
			, public isound_element
		{
		private:
			sound_engine_t _engine;
			sound_element_type_t _type;
			wstring _source_file_name;
			bool _sound_effect_started;

			XAUDIO2_BUFFER _audio_buffer;
			IXAudio2SourceVoice* _sound_effect_source_voice;
			array<byte> _sound_effect_buffer;
			uint _sound_effect_sample_rate;
			StreamingVoiceContext _voice_context;

		public:
			sound_element(sound_element_t engine, sound_element_type_t type, wstring file);

		public:
			XAUDIO2_BUFFER* audio_buffer()const { return const_cast<XAUDIO2_BUFFER*>(&_audio_buffer); }
			IXAudio2SourceVoice* sound_effect_source_voice()const { return _sound_effect_source_voice; };
			const_array<byte> sound_effect_buffer()const { return _sound_effect_buffer.is_empty() ? null : const_array<byte>((byte*)_sound_effect_buffer->buffer_ptr(), _sound_effect_buffer->buffer_size() - 1); }
			uint sound_effect_buffer_size()const { 
				return _sound_effect_buffer.is_empty() ? 0U : _sound_effect_buffer->buffer_size() - 1;
			}
			void sound_effect_buffer_size(uint value) { 
				if(_sound_effect_buffer.is_empty())
					_sound_effect_buffer = new collections::array_buffer<byte>();
				_sound_effect_buffer->size(value);
			}
			bool is_valid()const;

		public:
			virtual sound_element_type_t type()const override;
			virtual bool is_playing()const override;
			virtual wstring source_file()const override;
			virtual float volume()const override;
			virtual void volume(float) override;
			virtual float pitch()const override;
			virtual void pitch(float) override;

			virtual bool play() override;
			virtual bool pause() override;
			virtual bool stop() override;


		protected:
			void startup(sound_element_type_t type, wstring file);
			void recover();
			void shutdown();

		private:
			void on_shutdown_engine_event(objptr sender, objptr e);
			void on_engine_recovered_event(objptr sender, objptr e);

		public:
			virtual~sound_element();

		};

	}
}