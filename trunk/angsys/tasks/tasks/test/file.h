#pragma once

namespace ang
{
	namespace core
	{
		ang_object(file);
		ang_object(text_input_file);
		ang_object(text_output_file);
		
		class file :
			public object
		{
		protected:
			pointer handle;

		protected:
			file();
			virtual~file();

		public:

			ANG_DECLARE_INTERFACE();

			virtual bool open(wstring) = 0;
			virtual void clear()override;

			virtual bool is_eof()const;

			virtual bool is_valid()const;
			virtual long64 cursor()const;
			virtual void cursor(long64);

			virtual long64 size()const;
			virtual void size(long64);//write mode only

			virtual wsize read(pointer, wsize);//binary read
			virtual wsize write(pointer, wsize);//binary write
		};

		class text_input_file :
			public file
		{
		private:
			text::encoding_t e;
		public:
			text_input_file();

			ANG_DECLARE_INTERFACE();
			virtual bool open(wstring) override;

			void seek(raw_cstr_t);

		private:
			
			virtual void size(long64)override;

		private:
			virtual~text_input_file();
		};

		class text_output_file :
			public file
		{
		public:
			text_output_file();
			ANG_DECLARE_INTERFACE();
			virtual bool open(wstring) override;

		private:
			virtual~text_output_file();
		};
	}

}



