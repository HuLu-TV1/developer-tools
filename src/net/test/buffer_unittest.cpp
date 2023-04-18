#include <buffer.h>
#include <string>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace net;
BOOST_AUTO_TEST_CASE(test_BufferAppendRetrieve)
{
    Buffer buf ;
    BOOST_CHECK_EQUAL(buf.ReadableBytes(), 0);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(), Buffer::KInitiaSize);
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend);

    std::string str(200,'x');
    buf.Append(str);
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),str.size());
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize-buf.ReadableBytes());
    BOOST_CHECK_EQUAL(buf.PrependableBytes(),Buffer::kCheapPrepend);

    const std::string str2 =  buf.RetrieveAsString(50);
    BOOST_CHECK_EQUAL(str2.size(),50);
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),str.size()-str2.size());
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize - str.size());
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend+str2.size());
    BOOST_CHECK_EQUAL(str2, std::string(50, 'x'));

    buf.Append(str);
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),str.size() *2-str2.size());
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize - 2 *str.size());
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend+str2.size());

   const std::string str3 =  buf.RetrieveAllAsString();
    BOOST_CHECK_EQUAL(str3.size(),350);
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),0);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize );
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend);
    BOOST_CHECK_EQUAL(str3, std::string(350, 'x'));
}
BOOST_AUTO_TEST_CASE(test_BufferGrow){
    Buffer buf;
    buf.Append(std::string(400,'y'));
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),400);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize-400);
    buf.Retrieve(50);
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),350);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),Buffer::KInitiaSize-400);
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend+50);

    buf.Append(std::string(1000,'z'));
    BOOST_CHECK_EQUAL(buf.ReadableBytes(),1350);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(),0);
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend+50);

    buf.RetrieveAll();
    BOOST_CHECK_EQUAL(buf.ReadableBytes(), 0);
    BOOST_CHECK_EQUAL(buf.WriteableBytes(), 1400); 
    BOOST_CHECK_EQUAL(buf.PrependableBytes(), Buffer::kCheapPrepend);
}