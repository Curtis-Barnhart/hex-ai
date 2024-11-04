# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

from collections.abc import Generator, Iterable
import sys

import numpy as np
import tensorflow as tf


def read_in_bools(filename: str) -> Generator[np.ndarray, None, None]:
    with open(filename, "r") as file:
        for line in file:
            yield np.array((int(line[0]),))


def read_in_states(filename: str) -> Generator[np.ndarray, None, None]:
    with open(filename, "r") as file:
        for line in file:
            values = []
            for char in line[:-1]:
                match char:
                    case "0":
                        values.extend((1, 0, 0))
                    case "1":
                        values.extend((0, 1, 0))
                    case "2":
                        values.extend((0, 0, 1))
                    case _:
                        raise ValueError(f"Bad value {char} encountered reading in GameState::HexState.")

            yield np.array(values)


def write_to_tfrecord(data: Iterable[np.ndarray], labels: Iterable[np.ndarray], filename: str) -> None:
    # TODO: Okay obviously it would be good if we could store the gamestate with something like
    # `2` instead of (0, 0, 1) but I don't know how to get this working with the data pipeline!
    # If I have the time I'd love to figure this out, as well as a way to not store everything
    # as a SIXTY FOUR BIT INT???
    # data shape: (x, 363)
    # labels shape: (x, 1)
    print("hex-ai: Writing tfrecord to " + filename)
    with tf.io.TFRecordWriter(filename) as writer:
        for feature_array, label_array in zip(data, labels):
            feature = {
                "feature": tf.train.Feature(int64_list=tf.train.Int64List(value=feature_array)),
                "label": tf.train.Feature(int64_list=tf.train.Int64List(value=label_array))
            }
            proto = tf.train.Example(features=tf.train.Features(feature=feature))
            writer.write(proto.SerializeToString())


def _parse_from_proto(proto):
    feature_description = {
        "feature": tf.io.FixedLenFeature(363, tf.int64),
        "label": tf.io.FixedLenFeature(1, tf.int64)
    }
    parsed_features = tf.io.parse_single_example(proto, feature_description)

    # labels were "un-hot-coded" going in, so we must one_hot them here
    # to get back their full size
    labels = parsed_features["label"]
    one_hot_label = tf.one_hot(labels[0], depth=2)

    return tf.reshape(parsed_features["feature"], (1, 363)), tf.reshape(one_hot_label, (1, 2))


def read_from_tfrecords(input_filenames: Iterable[str]):
    ds = tf.data.TFRecordDataset(input_filenames, buffer_size=100000000, num_parallel_reads=16)
    ds = ds.map(_parse_from_proto)
    return ds


def main():
    print("hex-ai: starting conversion to text file type")
    states = read_in_states(sys.argv[1])
    labels = read_in_bools(sys.argv[2])
    write_to_tfrecord(states, labels, sys.argv[3])


if __name__ == "__main__":
    main()

