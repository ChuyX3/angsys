/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_ISTREAM_HPP__
#define __ANG_ISTREAM_HPP__

namespace ang
{
	namespace streams
	{
		ANG_INTERFACE(iinput_stream);
		ANG_INTERFACE(ioutput_stream);
		ANG_INTERFACE(itext_input_stream);
		ANG_INTERFACE(itext_output_stream);
		ANG_INTERFACE(ibinary_input_stream);
		ANG_INTERFACE(ibinary_output_stream);
	}
}

#endif//__ANG_STREAMS_HPP__
