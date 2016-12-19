#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef uint8_t ui8;
typedef uint16_t ui16;

typedef struct ip_header {
  ui8  version;
  ui8  ip_header_length;
  ui8  type_of_service;
  ui16 total_length;
  ui16 identification;
  ui8  flags;
  ui16 fragment_offset;
  ui8  time_to_live;
  ui8  protocol;
  ui16 header_checksum;
  uint32_t source_addr;
  uint32_t dest_addr;
  // Ignoring options
} ip_header;

#define MIN_IP_HEADER_WORD_SIZE 5

#define IP_HEADER_LEN_MASK 0x0f000000
#define IP_HEADER_LEN_SHIFT 24
#define TOTAL_LEN_MASK 0x0000ffff
#define TOTAL_LEN_SHIFT 0

#define OPTIONS_AND_PAD_BYTES 4

int read_ip_header(FILE *fp, ip_header *iph) {
  uint32_t word[MIN_IP_HEADER_WORD_SIZE];
  fread(word,sizeof(uint32_t),MIN_IP_HEADER_WORD_SIZE,fp);

  if (feof(fp)) {
    return EOF;
  }

  int err = ferror(fp);
  if (err != 0) {
    return err;
  }

  for (int i = 0; i < MIN_IP_HEADER_WORD_SIZE; i++)
    word[i] = ntohl(word[i]);

  iph->ip_header_length = (word[0] & IP_HEADER_LEN_MASK) >> IP_HEADER_LEN_SHIFT;
  iph->total_length = (word[0] & TOTAL_LEN_MASK) >> TOTAL_LEN_SHIFT;
  iph->source_addr = word[3];
  iph->dest_addr = word[4];

  // Seek ahead by 4 bytes to skip over options and padding if necessary.
  if (iph->ip_header_length > MIN_IP_HEADER_WORD_SIZE) {
    int success = fseek(fp,(iph->ip_header_length - MIN_IP_HEADER_WORD_SIZE)*4,SEEK_CUR);
    if (success != 0) {
      return success;
    }
  }

  return ferror(fp);
}

typedef struct tcp_header {
  ui16 source_port;
  ui16 dest_port;
  uint32_t sequence_number;
  uint32_t ack_number;
  ui8  data_offset;
  ui8  ctrl_bits;
  ui16 window;
  ui16 checksum;
  ui16 urgent_pointer;
  // Ignoring options
} tcp_header;

#define MIN_TCP_HEADER_WORD_SIZE 5

#define DATA_OFFSET_MASK 0xf0000000
#define DATA_OFFSET_SHIFT 28

int read_tcp_header(FILE *fp, tcp_header *tcph) {
  uint32_t word[MIN_TCP_HEADER_WORD_SIZE];
  size_t num_read = fread(word,sizeof(uint32_t),MIN_TCP_HEADER_WORD_SIZE,fp);

  int err = ferror(fp);
  if (err != 0) {
    return err;
  }
  // If there wasn't an error but we didn't read the right number of bytes, we
  // hit the EOF when we shouldn't have.
  if (num_read != MIN_TCP_HEADER_WORD_SIZE) {
    return EOF;
  }

  for (int i = 0; i < MIN_TCP_HEADER_WORD_SIZE; i++)
    word[i] = ntohl(word[i]);

  tcph->data_offset = (word[3] & DATA_OFFSET_MASK) >> DATA_OFFSET_SHIFT;

  // Seek ahead by 4 bytes to skip over options and padding if necessary.
  if (tcph->data_offset > MIN_TCP_HEADER_WORD_SIZE) {
    int success = fseek(fp,(tcph->data_offset - MIN_TCP_HEADER_WORD_SIZE)*4,SEEK_CUR);
    if (success != 0) {
      return success;
    }
  }

  return ferror(fp);
}

int write_data(FILE *in, int len, FILE *out) {
  char *bytes = malloc(sizeof(char)*len);
  int r = 0, w = 0;

  r = fread(bytes,sizeof(char),len,in);

  while (w<r) {
    char *offset = bytes + w;
    int total = fwrite(offset,sizeof(char),r-w,out);
    if (total < 0) {
      return total;
    }
    w += total;
  }

  free(bytes);
  return 0;
}

int main(int argc, char *argv[]) {
  FILE *fpin, *fpout;

  if (argc != 3) {
    perror("Usage: extract <fin> <fout>");
    return 1;
  }

  if ((fpin=fopen(argv[1],"rb")) == 0) {
    perror("Cannot find file to extract");
    return 2;
  }

  if ((fpout=fopen(argv[2],"wb")) == 0) {
    perror("Cannot open output file");
    return 3;
  }

  ip_header iph;
  tcp_header tcph;
  memset(&iph,0,sizeof(iph));
  memset(&tcph,0,sizeof(tcph));

  while(!feof(fpin)) {
    int err = read_ip_header(fpin, &iph);
    if (err == EOF) {
      break; // leave while loop
    } else if (err != 0) {
      perror("Error reading IP header from logfile");
      return 4;
    }
    if (read_tcp_header(fpin, &tcph) != 0) {
      perror("Error reading TCP header from logfile");
      return 5;
    }

    // The length of the data in this packet.
    int data_len = iph.total_length - (4*iph.ip_header_length) - (4*tcph.data_offset);
    if (write_data(fpin, data_len, fpout) != 0) {
      perror("Error reading data from logfile");
      return 6;
    }
  }

  fclose(fpin);
  fclose(fpout);

  return 0;
}
