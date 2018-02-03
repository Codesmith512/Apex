#include "pic"

#include "interrupts"
#include "ports"

#define PIC1_CMD   0x20
#define PIC1_DATA  0x21
#define PIC2_CMD   0xa0
#define PIC2_DATA  0xa1

/** Enables the PIC, but with all IRQs masked */
void pic::initialize()
{
  /* Initialization Command */
  ports::out_8(PIC1_CMD, 0x11);
  ports::io_wait();
  ports::out_8(PIC2_CMD, 0x11);
  ports::io_wait();

  /* Setup IRQs at INT 0x20~0x2f */
  ports::out_8(PIC1_DATA, 0x20);
  ports::io_wait();
  ports::out_8(PIC2_DATA, 0x28);
  ports::io_wait();

  /* Inform master it has a slave at IRQ2 (0000 0100) */
  ports::out_8(PIC1_DATA, 0x04);
  ports::io_wait();

  /* Give slave it's cascade identity (0000 0010) */
  ports::out_8(PIC2_DATA, 0x02);
  ports::io_wait();

  /* Set 8086/8088 mode */
  ports::out_8(PIC1_DATA, 0x01);
  ports::io_wait();
  ports::out_8(PIC2_DATA, 0x01);
  ports::io_wait();

  /* Set interrupt masks */
  mask(irq_t::ALL);
}

/* Unmask IRQ */
void pic::unmask(irq_t irqs)
{
  ports::out_8(PIC1_DATA, ports::in_8(PIC1_DATA) & ~static_cast<uint8_t>(irqs & 0xff));
  ports::out_8(PIC2_DATA, ports::in_8(PIC2_DATA) & ~static_cast<uint8_t>((irqs >> 8) & 0xff));
}

/* Mask IRQ */
void pic::mask(irq_t irqs)
{
  ports::out_8(PIC1_DATA, ports::in_8(PIC1_DATA) | static_cast<uint8_t>(irqs & 0xff));
  ports::out_8(PIC2_DATA, ports::in_8(PIC2_DATA) | static_cast<uint8_t>((irqs >> 8) & 0xff));
}

/* Acknowledge IRQ */
void pic::acknowledge(irq_t irq)
{
  ports::out_8(PIC1_CMD, 0x20);
  if(irq > 0xff)
    ports::out_8(PIC2_CMD, 0x20);
}

/* Obtain Interrupt Request Register */
pic::irq_t pic::get_irr()
{
  ports::out_8(PIC1_CMD, 0x0a);
  ports::out_8(PIC2_CMD, 0x0a);

  return (ports::in_8(PIC1_CMD)) |
         (ports::in_8(PIC2_CMD) << 8);
}

/* Obtain Interrupt Status Register */
pic::irq_t pic::get_isr()
{
  ports::out_8(PIC1_CMD, 0x0b);
  ports::out_8(PIC2_CMD, 0x0b);

  return (ports::in_8(PIC1_CMD)) |
         (ports::in_8(PIC2_CMD) << 8);
}
