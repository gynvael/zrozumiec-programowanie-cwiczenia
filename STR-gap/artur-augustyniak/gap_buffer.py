#!/usr/bin/env python
# -*- coding: utf-8 -*-


class GapBuffer(object):

    def __init__(self):
        self.BUFF_INIT_SIZE = 16

        self.buffer = [''] * self.BUFF_INIT_SIZE
        self.last_pos = 0
        self.gap_size = 8
        self.min_gap_size_threshold = 2
        self.gap_size_reminder = self.gap_size - self.min_gap_size_threshold
        self.last_buff_end = self.BUFF_INIT_SIZE - self.gap_size

    def move_to_position(self, pos):
        if pos < self.last_pos:
            for x in range(self.last_pos - 1, pos - 1, -1):
                self.buffer[x + self.gap_size] = self.buffer[x]
                self.buffer[x] = ''
        elif pos > self.last_pos:
            for y in range(self.last_pos, pos):
                self.buffer[y] = self.buffer[y + self.gap_size]
                self.buffer[y + self.gap_size] = ''
        self.last_pos = pos

    def insert_char(self, c):
        self.buffer[self.last_pos] = c
        self.last_pos += 1
        self.gap_size -= 1
        if self.gap_size <= self.min_gap_size_threshold:
            last_buff_size = len(self.buffer) - 1
            chunk_size = last_buff_size - self.last_pos
            self.buffer += [''] * self.gap_size_reminder
            self.last_buff_end += self.gap_size_reminder
            for x in range(chunk_size, 0, -1):
                self.buffer[self.last_pos + x + self.gap_size_reminder] = self.buffer[self.last_pos + x]
                self.buffer[self.last_pos + x] = ''
            self.gap_size += self.gap_size_reminder

    def delete_char(self):
        self.buffer[self.last_pos - 1] = ''
        self.last_pos -= 1
        self.last_buff_end -= 1
        if self.gap_size > self.min_gap_size_threshold + self.gap_size_reminder - 1:
            rev_point = self.last_pos + self.gap_size
            for x in range(rev_point, len(self.buffer)):
                self.buffer[x - 1] = self.buffer[x]
        else:
            self.gap_size += 1
        self.check_shrink_buffer()

    def check_shrink_buffer(self):
        if self.last_buff_end <= len(self.buffer) / 4:
            del self.buffer[-self.last_buff_end:]

    def dump_buffer(self):
        print "####################"
        print self.buffer


if __name__ == "__main__":
    gb = GapBuffer()
    gb.dump_buffer()
    for ch in xrange(0, 123):
        gb.insert_char(ch)
    gb.dump_buffer()

    gb.move_to_position(6)
    gb.insert_char("X")
    gb.insert_char("X")
    gb.insert_char("X")
    gb.insert_char("X")
    gb.insert_char("X")
    gb.insert_char("X")
    gb.dump_buffer()

    gb.move_to_position(30)
    gb.insert_char("U")
    gb.dump_buffer()

    gb.delete_char()
    gb.delete_char()
    gb.delete_char()
    gb.delete_char()
    gb.delete_char()
    gb.delete_char()
    gb.dump_buffer()

    gb.move_to_position(6)
    gb.insert_char("E")
    gb.dump_buffer()
    gb.move_to_position(gb.last_buff_end)
    for x in range(gb.last_pos, 3, -1):
        gb.delete_char()
    gb.dump_buffer()
    gb.insert_char("A")
    gb.insert_char("B")
    gb.insert_char("C")
    gb.insert_char("D")
    gb.insert_char("F")
    gb.insert_char("G")
    gb.dump_buffer()