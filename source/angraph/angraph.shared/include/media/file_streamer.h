#pragma once

namespace ang
{
	namespace media
	{
		class file_streamer;
		typedef object_wrapper<file_streamer> file_streamer_t;

		class file_streamer
			: public object
			, public IMFByteStream
		{
		private:
			ulong _seek;
			streams::stream_mode_t _mode;
			core::files::binary_file_t _file;
			core::async::iasync_t<bool> _streaming_task;

		public:
			file_streamer(core::files::file_t);
			file_streamer(cwstr_t, streams::stream_mode_t);

		public: //IMFByteStream overrides
			virtual long STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
			virtual dword STDMETHODCALLTYPE AddRef(void) override;
			virtual dword STDMETHODCALLTYPE Release(void) override;
			virtual long STDCALL GetCapabilities(dword *pdwCapabilities) override;
			virtual long STDCALL GetLength(qword *pqwLength) override;
			virtual long STDCALL SetLength(qword qwLength) override;
			virtual long STDCALL GetCurrentPosition(qword *pqwPosition) override;
			virtual long STDCALL SetCurrentPosition(qword qwPosition) override;
			virtual long STDCALL IsEndOfStream(BOOL *pfEndOfStream) override;
			virtual long STDCALL Read(byte *pb, ulong cb, ulong *pcbRead) override;
			virtual long STDCALL BeginRead(byte *pb, ulong cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;
			virtual long STDCALL EndRead(IMFAsyncResult *pResult, ulong *pcbRead) override;
			virtual long STDCALL Write(const byte *pb, ulong cb, ulong *pcbWritten) override;
			virtual long STDCALL BeginWrite(const byte *pb, ulong cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;
			virtual long STDCALL EndWrite(IMFAsyncResult *pResult, ulong *pcbWritten) override;
			virtual long STDCALL Seek(MFBYTESTREAM_SEEK_ORIGIN SeekOrigin, long64 llSeekOffset, dword dwSeekFlags, qword *pqwCurrentPosition) override;
			virtual long STDCALL Flush(void) override;
			virtual long STDCALL Close(void) override;

		private:
			virtual~file_streamer();
		};





		class input_byte_stream
			: public object
			, public IMFByteStream
		{
		private:
			com_wrapper<IMFByteStream> m_byteStream;

		public:
			input_byte_stream(cwstr_t);

		public: //IMFByteStream overrides
			virtual long STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
			virtual dword STDMETHODCALLTYPE AddRef(void) override;
			virtual dword STDMETHODCALLTYPE Release(void) override;
			virtual long STDCALL GetCapabilities(dword *pdwCapabilities) override;
			virtual long STDCALL GetLength(qword *pqwLength) override;
			virtual long STDCALL SetLength(qword qwLength) override;
			virtual long STDCALL GetCurrentPosition(qword *pqwPosition) override;
			virtual long STDCALL SetCurrentPosition(qword qwPosition) override;
			virtual long STDCALL IsEndOfStream(BOOL *pfEndOfStream) override;
			virtual long STDCALL Read(byte *pb, ulong cb, ulong *pcbRead) override;
			virtual long STDCALL BeginRead(byte *pb, ulong cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;
			virtual long STDCALL EndRead(IMFAsyncResult *pResult, ulong *pcbRead) override;
			virtual long STDCALL Write(const byte *pb, ulong cb, ulong *pcbWritten) override;
			virtual long STDCALL BeginWrite(const byte *pb, ulong cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;
			virtual long STDCALL EndWrite(IMFAsyncResult *pResult, ulong *pcbWritten) override;
			virtual long STDCALL Seek(MFBYTESTREAM_SEEK_ORIGIN SeekOrigin, long64 llSeekOffset, dword dwSeekFlags, qword *pqwCurrentPosition) override;
			virtual long STDCALL Flush(void) override;
			virtual long STDCALL Close(void) override;

		private:
			virtual~input_byte_stream();
		};



	}
}