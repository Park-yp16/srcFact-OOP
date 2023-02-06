/*
    refillContent.cpp
    
    Implementation file for refillContent functions
*/

#include "refillContent.hpp"
#include <errno.h>
#include <algorithm> 

#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#define READ read
#else
#include <BaseTsd.h>
#include <io.h>
typedef SSIZE_T ssize_t;
#define READ _read
#endif

const int BLOCK_SIZE = 4096;
const int BUFFER_SIZE = 16 * 16 * BLOCK_SIZE;

/*
    Refill the content preserving the existing data.

    @param[in, out] content View of the content
    @return Number of bytes read
    @retval 0 EOF
    @retval -1 Read error
*/
[[nodiscard]] int refillContent(std::string_view& data) {

    // initialize the internal buffer at first use
    static char buffer[BUFFER_SIZE];

    // preserve prefix of unprocessed characters to start of the buffer
    std::copy(data.cbegin(), data.cend(), buffer);

    // read in multiple of whole blocks
    ssize_t bytesRead = 0;
    while (((bytesRead = READ(0, (buffer + data.size()),
        BUFFER_SIZE - BLOCK_SIZE)) == -1) && (errno == EINTR)) {
    }
    if (bytesRead == -1) {
        // error in read
        return -1;
    }

    // set content to the start of the buffer
    data = std::string_view(buffer, data.size() + bytesRead);

    return bytesRead;
}
