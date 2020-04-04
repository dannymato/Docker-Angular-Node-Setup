
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

static struct nf_hook_ops *nfho = NULL;

void ip_to_string(int, char*);
struct file *file_open(const char*, int, int);
void file_close(struct file*);
int file_read(struct file*, unsigned long long, unsigned char*, unsigned int);
int file_write(struct file*, unsigned long long, unsigned char*, unsigned int);
int length_test(unsigned char[4]);


static unsigned int hfunc (void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {

    struct iphdr *iph;
    if (!skb)
        return NF_ACCEPT;

    iph = ip_hdr(skb);

    int dest_ip_int = __be32_to_cpu(iph->daddr);
    int source_ip_int = __be32_to_cpu(iph->saddr);

    char dest[15];
    char source[15];

    ip_to_string(dest_ip_int, dest);
    ip_to_string(source_ip_int, source);

    const char* comma = ",";

    char to_write[35];

    // Dest_IP,SourceIP
    strlcpy(to_write, dest, 35 * sizeof(char));
    strlcat(to_write, comma, 35 * sizeof(char));
    strlcat(to_write, source, 35 * sizeof(char));
    strlcat(to_write, "\n", 35 * sizeof(char));

    struct file* logfile = file_open("/tmp/netfilterlog", O_RDWR | O_APPEND | O_CREAT | O_NONBLOCK, S_IRWXU);

    if (logfile) {

        int length;
        length = strlen(to_write);

        file_write(logfile, 0, to_write, length);

        file_close(logfile);

    }
    else {
        printk("Could not write to file");
    }

    return NF_ACCEPT;

}

static int __init LKM_init(void) {
    nfho = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);

    nfho->hook = (nf_hookfn*)hfunc;
    nfho->hooknum = NF_INET_PRE_ROUTING;
    nfho->pf = PF_INET;
    nfho->priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, nfho);

    return 0;
}

static void __exit LKM_exit(void) {
    nf_unregister_net_hook(&init_net, nfho);
    kfree(nfho);

    struct file* logfile = file_open("/tmp/netfilterlog", O_WRONLY | O_TRUNC, NULL);
    if (logfile) {
        file_close(logfile);
    }
}

void ip_to_string(int ip, char* buffer) {

    unsigned char ip_bytes[4];
    ip_bytes[0] = ip & 0xFF;
    ip_bytes[1] = (ip >> 8) & 0xFF;
    ip_bytes[2] = (ip >> 16) & 0xFF;
    ip_bytes[3] = (ip >> 24) & 0xFF;

    int length  = length_test(ip_bytes);

    snprintf(buffer, length + 1, "%d.%d.%d.%d", ip_bytes[3], ip_bytes[2], ip_bytes[1], ip_bytes[0]);

}

int length_test(unsigned char bytes[4]) {

    int num_digits = 0;

    int i;
    for (i = 0; i < 4; i++) {
        if (bytes[i] >= 100) {
            num_digits += 3;
        }
        else if (bytes[i] >= 10) {
            num_digits += 2;
        }
        else {
            num_digits += 1;
        }
    }

    return num_digits + 3;

}

struct file *file_open(const char *path, int flags, int rights) {

    struct file *filp = NULL;
    int err = 0;

    filp = filp_open(path, flags, rights);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        printk("File open error 0x%lx", err);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) {
    filp_close(file, NULL);
}

int file_read(struct file *file, unsigned long long offset, unsigned char* data, unsigned int size) {
    
    int ret;

    ret = kernel_read(file, data, size, &offset);
    return ret;

}

int file_write(struct file *file, unsigned long long offset, unsigned char* data, unsigned int size) {
    int ret;
    ret = kernel_write(file, data, size, &offset);

    return ret;

}


module_init(LKM_init);
module_exit(LKM_exit);
MODULE_LICENSE("GPL");