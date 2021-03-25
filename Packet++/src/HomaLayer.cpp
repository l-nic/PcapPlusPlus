#include "HomaLayer.h"
#include "AppLayer.h"
#include <string.h>
#include <sstream>

namespace pcpp
{

HomaLayer::HomaLayer(uint8_t flags, uint16_t src_context, uint16_t dst_context,
                     uint16_t msg_len, uint8_t pkt_offset,
                     uint16_t grant_offset, uint8_t grant_prio, uint16_t tx_msg_id,
                     uint16_t buf_ptr, uint8_t buf_size_class) {
    const size_t header_len = sizeof(homahdr);
    m_DataLen = header_len;
    m_Data = new uint8_t[header_len];
    memset(m_Data, 0, header_len);
    homahdr* homa_hdr = (homahdr*)m_Data;
    m_Protocol = HOMA;
    homa_hdr->flags = flags;
    homa_hdr->src_context = htobe16(src_context);
    homa_hdr->dst_context = htobe16(dst_context);
    homa_hdr->msg_len = htobe16(msg_len);
    homa_hdr->pkt_offset = pkt_offset;
    homa_hdr->grant_offset = htobe16(pull_offset);
    homa_hdr->grant_prio = grant_prio;
    homa_hdr->tx_msg_id = htobe16(tx_msg_id);
    homa_hdr->buf_ptr = htobe16(buf_ptr);
    homa_hdr->buf_size_class = buf_size_class;
}

HomaLayer::HomaLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet) : Layer(data, dataLen, prevLayer, packet) {
    m_Protocol = HOMA;
    m_DataLen = sizeof(homahdr) + be16toh(getHomaHeader()->msg_len);
}

void HomaLayer::parseNextLayer() {
    size_t header_len = getHeaderLen();
    if (m_DataLen <= header_len) {
        return;
    }
    uint8_t* payload = m_Data + header_len;
    size_t payload_len = m_DataLen - header_len;
    m_NextLayer = new AppLayer(payload, payload_len, this, m_Packet);
}

std::string HomaLayer::toString() const {
    std::ostringstream output_string;
    std::string flags_str;
    homahdr* homa_hdr = getHomaHeader();
    uint8_t homa_header_flags = homa_hdr->flags;
    bool is_data = homa_header_flags & HOMA_DATA_FLAG_MASK;
    bool is_ack = homa_header_flags & HOMA_ACK_FLAG_MASK;
    bool is_nack = homa_header_flags & HOMA_NACK_FLAG_MASK;
    bool is_pull = homa_header_flags & HOMA_GRANT_FLAG_MASK;
    bool is_chop = homa_header_flags & HOMA_CHOP_FLAG_MASK;
    bool is_rsnd = homa_header_flags & HOMA_RESEND_FLAG_MASK;
    bool is_busy = homa_header_flags & HOMA_BUSY_FLAG_MASK;
    bool is_bogus = homa_header_flags & HOMA_BOGUS_FLAG_MASK;
    flags_str += is_data ? "DATA" : "";
    flags_str += is_ack ? " ACK" : "";
    flags_str += is_nack ? " NACK" : "";
    flags_str += is_pull ? " PULL" : "";
    flags_str += is_chop ? " CHOP" : "";
    flags_str += is_rsnd ? " RSND" : "";
    flags_str += is_busy ? " BUSY" : "";
    flags_str += is_bogus ? " BOGUS" : "";
    output_string << "HOMA(flags=" << flags_str << ", src_context=" << be16toh(homa_hdr->src_context)
                  << ", dst_context=" << be16toh(homa_hdr->dst_context) << ", msg_len="
                  << be16toh(homa_hdr->msg_len) << ", pkt_offset=" << (int)homa_hdr->pkt_offset
                  << ", grant_offset=" << be16toh(homa_hdr->grant_offset) << ", grant_prio=" 
                  << (int)homa_hdr->grant_prio << ", tx_msg_id=" << be16toh(homa_hdr->tx_msg_id) 
                  << ", buf_ptr=" << be16toh(homa_hdr->buf_ptr) << ", buf_size_class=" 
                  << (int)homa_hdr->buf_size_class << ")";
    return output_string.str();
}

}