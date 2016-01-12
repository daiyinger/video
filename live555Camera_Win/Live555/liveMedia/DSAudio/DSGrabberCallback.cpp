#include "General.h"
#include "DSGrabberCallback.h"

DSGrabberCallback::DSGrabberCallback()
    : is_buffer_available_(false)
	, m_pCycleBuffer(NULL)
{
    buffer_ = NULL;
    buffer_size_ = 0;

	m_pCycleBuffer = new CQueueBuffer(10000*2);
}

DSGrabberCallback::~DSGrabberCallback()
{
    SAFE_DELETE_ARRAY(buffer_);

	if (m_pCycleBuffer)
	{
		delete m_pCycleBuffer;
		m_pCycleBuffer = NULL;
	}
}

STDMETHODIMP_(ULONG) DSGrabberCallback::AddRef()
{
    return 2;
}

STDMETHODIMP_(ULONG) DSGrabberCallback::Release()
{
    return 1;
}

STDMETHODIMP DSGrabberCallback::QueryInterface(REFIID riid, void ** ppv)
{
    if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
    {
        *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
        return NOERROR;
    }    

    return E_NOINTERFACE;
}

STDMETHODIMP DSGrabberCallback::SampleCB(double SampleTime, IMediaSample* pSample)
{
    return 0;
}

STDMETHODIMP DSGrabberCallback::BufferCB(double dblSampleTime, BYTE *pBuffer, long lBufferSize)
{
    if (!pBuffer)
    {
        return E_POINTER;
    }

    //semaphore_.Wait();
    base::AutoLock al(lock_);

    timestamp_ = ::GetTickCount();

#ifdef OLD_BUFFER

     if( buffer_size_ < lBufferSize )
     {
         SAFE_DELETE_ARRAY( buffer_ );
         buffer_size_ = 0;
     }
 
     if (!buffer_)
     {
         buffer_ = new char[lBufferSize];
         buffer_size_ = lBufferSize;
     }
 
     if( !buffer_ )
     {
         buffer_size_ = 0;
         //semaphore_.Post();
         return E_OUTOFMEMORY;
     }
 
     memcpy( buffer_, pBuffer, lBufferSize );
	 //fwrite(buffer_, 1, buffer_size_, _file);

//     buffer_ = (char*)pBuffer;
//     buffer_size_ = lBufferSize;

#else

	if (m_pCycleBuffer != NULL && pBuffer != NULL && lBufferSize > 0)
		m_pCycleBuffer->Write((char*)pBuffer, lBufferSize);

#endif

    is_buffer_available_ = true;

    fps_meter_++;

    //semaphore_.Post();

    return 0;
}

bool DSGrabberCallback::IsBufferAvailable()
{
    return is_buffer_available_;
}

char* DSGrabberCallback::GetBuffer(int& bufLen)
{
#ifdef OLD_BUFFER

	if(!is_buffer_available_)
		return NULL;
    base::AutoLock al(lock_);

    char* buf = NULL;
    if (BufferSize())
    {
        buf = (char*)malloc(BufferSize());
		bufLen = BufferSize();
        memcpy(buf, buffer_, BufferSize());
    }

    return buf;

#else

	if (m_pCycleBuffer != NULL)
	{
		bufLen = 4096;//m_pCycleBuffer->GetFilledSize();
		char* buf = NULL;
		if (bufLen > 0)
		{
			buf = (char*)malloc(bufLen);
			bufLen = m_pCycleBuffer->Read(buf, bufLen);
		}

		return buf;
	}
	else
	{
		bufLen = 0;
		return NULL;
	}
	
#endif
}

void DSGrabberCallback::ReleaseBuffer(char* buf)
{
    is_buffer_available_ = false;
    if (buf) free(buf);
}
