#include "pch.h"
#include "ang/media/media.h"
#include "media/sound_engine.h"

using namespace ang;
using namespace ang::media;

class file_streamer
	: public object
	, public IMFByteStream
{
	core::files::input_binary_file_t file;
	wsize _readed;
	core::async::iasync_t<wsize> _reading;
public:
	file_streamer();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) {
		if (riid == __uuidof(IMFByteStream))
		{
			if (ppvObject) {
				*ppvObject = static_cast<IMFByteStream*>(this);
				AddRef();
			}
			return S_OK;
		}
		else if (riid == __uuidof(IUnknown))
		{
			if (ppvObject) {
				*ppvObject = static_cast<IMFByteStream*>(this);
				AddRef();
			}
			return S_OK;
		}
		return -1;
	}
	virtual ULONG STDMETHODCALLTYPE AddRef(void) { return add_ref(); }
	virtual ULONG STDMETHODCALLTYPE Release(void) { return release(); }

	virtual HRESULT STDMETHODCALLTYPE GetCapabilities(DWORD *pdwCapabilities) override;

	virtual HRESULT STDMETHODCALLTYPE GetLength(QWORD *pqwLength) override;

	virtual HRESULT STDMETHODCALLTYPE SetLength(QWORD qwLength) override;

	virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition(QWORD *pqwPosition) override;

	virtual HRESULT STDMETHODCALLTYPE SetCurrentPosition(QWORD qwPosition) override;

	virtual HRESULT STDMETHODCALLTYPE IsEndOfStream(BOOL *pfEndOfStream) override;

	virtual HRESULT STDMETHODCALLTYPE Read(BYTE *pb, ULONG cb, ULONG *pcbRead) override;

	virtual HRESULT STDMETHODCALLTYPE BeginRead(BYTE *pb, ULONG cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;

	virtual HRESULT STDMETHODCALLTYPE EndRead(IMFAsyncResult *pResult, ULONG *pcbRead) override;

	virtual HRESULT STDMETHODCALLTYPE Write(const BYTE *pb, ULONG cb, ULONG *pcbWritten) override;

	virtual HRESULT STDMETHODCALLTYPE BeginWrite(const BYTE *pb, ULONG cb, IMFAsyncCallback *pCallback, IUnknown *punkState) override;

	virtual HRESULT STDMETHODCALLTYPE EndWrite(IMFAsyncResult *pResult, ULONG *pcbWritten) override;

	virtual HRESULT STDMETHODCALLTYPE Seek(MFBYTESTREAM_SEEK_ORIGIN SeekOrigin, LONGLONG llSeekOffset, DWORD dwSeekFlags, QWORD *pqwCurrentPosition) override;

	virtual HRESULT STDMETHODCALLTYPE Flush(void) override;

	virtual HRESULT STDMETHODCALLTYPE Close(void) override;

};


media_streamer::media_streamer()
{
	_reader = nullptr;
	_audio_type = nullptr;
	ZeroMemory(&_wave_format, sizeof(_wave_format));
}

media_streamer::~media_streamer()
{
}

void media_streamer::init(cwstr_t url)
{
	com_wrapper<IMFMediaType> outputMediaType;
	com_wrapper<IMFMediaType> mediaType;

	//MFCreateFile()

	//throw_if_failed(
	//    MFStartup(MF_VERSION)
	//    );

	throw_if_failed(
		MFCreateSourceReaderFromURL(url, nullptr, &_reader)
	);

	// Set the decoded output format as PCM.
	// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
	// When this sample uses Media Foundation, it always decodes into PCM.

	throw_if_failed(
		MFCreateMediaType(&mediaType)
	);

	throw_if_failed(
		mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)
	);

	throw_if_failed(
		mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)
	);

	throw_if_failed(
		_reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.get())
	);

	// get the complete WAVEFORMAT from the Media Type.
	throw_if_failed(
		_reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &outputMediaType)
	);

	uint formatSize = 0;
	WAVEFORMATEX* waveFormat;
	throw_if_failed(
		MFCreateWaveFormatExFromMFMediaType(outputMediaType.get(), &waveFormat, &formatSize)
	);
	CopyMemory(&_wave_format, waveFormat, sizeof(_wave_format));
	CoTaskMemFree(waveFormat);

	// get the total length of the stream, in bytes.
	PROPVARIANT var;
	throw_if_failed(
		_reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var)
	);

	// duration is in 100ns units; convert to seconds, and round up
	// to the nearest whole byte.
	ulong64 duration = var.uhVal.QuadPart;
	_max_stream_length_in_bytes =
		static_cast<unsigned int>(
		((duration * static_cast<ulong64>(_wave_format.nAvgBytesPerSec)) + 10000000) /
			10000000
			);
}

bool media_streamer::get_next_buffer(const_array<byte> buffer, uint& bufferLength)
{
	com_wrapper<IMFSample> sample;
	com_wrapper<IMFMediaBuffer> mediaBuffer;
	byte *audioData = nullptr;
	dword sampleBufferLength = 0;
	dword flags = 0;

	bufferLength = 0;

	if (_reader == nullptr)
	{
		return false;
	}

	throw_if_failed(
		_reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample)
	);

	if (sample == nullptr)
	{
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	throw_if_failed(
		sample->ConvertToContiguousBuffer(&mediaBuffer)
	);

	throw_if_failed(
		mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)
	);

	// Only copy the sample if the remaining buffer is large enough.
	if (sampleBufferLength <= buffer.size())
	{
		CopyMemory(buffer.data(), audioData, sampleBufferLength);
		bufferLength = sampleBufferLength;
	}

	if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void media_streamer::read_all(const_array<byte> buffer, uint& bufferLength)
{
	uint valuesWritten = 0;
	uint sampleBufferLength = 0;

	if (_reader == nullptr)
	{
		return;
	}

	bufferLength = 0;
	// If buffer isn't large enough, return
	if (buffer.size() < _max_stream_length_in_bytes)
	{
		return;
	}

	while (!get_next_buffer(buffer, sampleBufferLength))
	{
		valuesWritten += sampleBufferLength;
		buffer.set(buffer.get() + sampleBufferLength, buffer.size() - sampleBufferLength);
	}

	bufferLength = valuesWritten + sampleBufferLength;
}

void media_streamer::restart()
{
	if (_reader == nullptr)
	{
		return;
	}

	PROPVARIANT var = { 0 };
	var.vt = VT_I8;

	throw_if_failed(
		_reader->SetCurrentPosition(GUID_NULL, var)
	);
}

