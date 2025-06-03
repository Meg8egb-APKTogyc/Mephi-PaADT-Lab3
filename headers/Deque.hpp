#pragma once
#include "Sequence/Sequence.hpp"
#include "Sequence/SegmentedSequence.hpp"
#include <functional>


template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    virtual void PushFront(const T& item) = 0;
    virtual void PushBack(const T& item) = 0;
    virtual T PopFront() = 0;
    virtual T PopBack() = 0;
    virtual const T& Front() const = 0;
    virtual const T& Back() const = 0;

    virtual int Size() const = 0;
    virtual bool isEmpty() const = 0;

    Deque<T>* where(const std::function<bool(const T&)>& predicate) const {
        Deque<T>* result = this->cloneEmpty();
        Deque<T>* temp = this->clone();

        while (!temp->isEmpty()) {
            T item = temp->PopFront();
            if (predicate(item)) {
                result->PushBack(item);
            }
        }

        delete temp;
        return result;
    }

    Deque<T>* map(const std::function<T(const T&)>& func) const {
        Deque<T>* result = this->cloneEmpty();
        Deque<T>* temp = this->clone();

        while (!temp->isEmpty()) {
            T item = temp->PopFront();
            result->PushBack(func(item));
        }

        delete temp;
        return result;
    }

    T reduce(const std::function<T(const T&, const T&)>& func, T initial) const {
        Deque<T>* temp = this->clone();
        T accumulator = initial;

        while (!temp->isEmpty()) {
            T item = temp->PopFront();
            accumulator = func(accumulator, item);
        }

        delete temp;
        return accumulator;
    }

    Deque<T>* concat(const Deque<T>* other) const {
        Deque<T>* result = this->clone();
        Deque<T>* temp = other->clone();

        while (!temp->isEmpty()) {
            result->PushBack(temp->PopFront());
        }

        delete temp;
        return result;
    }

    Deque<T>* subSequence(int start, int end) const {
        Deque<T>* result = this->cloneEmpty();
        Deque<T>* temp = this->clone();

        for (int i = 0; i < start && !temp->isEmpty(); ++i) {
            temp->PopFront();
        }

        for (int i = start; i < end && !temp->isEmpty(); ++i) {
            result->PushBack(temp->PopFront());
        }

        delete temp;
        return result;
    }
    
    virtual Deque<T>* cloneEmpty() const = 0;

    Deque<T>* clone() const {
        Deque<T>* copy = cloneEmpty();
        Deque<T>* temp = cloneEmpty();
        
        Deque<T>* source = const_cast<Deque<T>*>(this);
        while (!source->isEmpty()) {
            T item = source->PopFront();
            temp->PushBack(item);
        }
    
        while (!temp->isEmpty()) {
            T item = temp->PopFront();
            source->PushBack(item);
            copy->PushBack(item);
        }

        delete temp;
        return copy;
    }
};


template<typename T>
class SequenceDeque : public Deque<T> {
private:
    ArraySequence<T>* seq;
    int fidx, lidx;
    int count;

    void Resize() {
        int newSize = seq->GetCapacity() == 0 ? 1 : seq->GetCapacity() * 2;
        ArraySequence<T>* newSeq = new MutableArraySequence<T>(newSize);
        
        for (int i = 0; i < count; ++i) {
            (*newSeq)[i] = seq->Get((fidx + i) % seq->GetCapacity());
        }
        
        delete seq;
        seq = newSeq;
        fidx = 0;
        lidx = count;
    }

    void checkResize() {
        if (seq->GetCapacity() == 0 || (lidx % seq->GetCapacity() == fidx && count != 0))
            this->Resize();
    }

public:
    SequenceDeque() : seq(new MutableArraySequence<T>(2)), fidx(1), lidx(1), count(0) {}

    ~SequenceDeque() override {
        delete seq;
    }

    virtual void PushFront(const T& item) override {
        checkResize();

        fidx = (fidx - 1 + seq->GetCapacity()) % seq->GetCapacity();
        (*seq)[fidx] = item;
        count++;
    }


    virtual void PushBack(const T& item) override {
        checkResize();

        (*seq)[lidx] = item;
        lidx = (lidx + 1) % seq->GetCapacity();
        count++;
    }

    virtual T PopFront() override {
        if (isEmpty())
            throw std::out_of_range("Deque is empty");

        T result = seq->Get(fidx);
        fidx = (fidx + 1) % seq->GetCapacity();
        count--;
        return result;
    }

    virtual T PopBack() override {
        if (isEmpty())
            throw std::out_of_range("Deque is empty");
            
        lidx = (lidx - 1 + seq->GetCapacity()) % seq->GetCapacity();
        T result = seq->Get(lidx);
        count--;
        return result;
    }

    virtual const T& Front() const override {
        if (isEmpty())
            throw std::out_of_range("Deque is empty");
        return seq->Get(fidx);
    }

    virtual const T& Back() const override {
        if (isEmpty())
            throw std::out_of_range("Deque is empty");
        return seq->Get((lidx - 1 + seq->GetCapacity()) % seq->GetCapacity());
    }

    virtual int Size() const override {
        return count;
    }

    virtual bool isEmpty() const override {
        return count == 0;
    }

    virtual Deque<T>* cloneEmpty() const override {
        return new SequenceDeque<T>();
    }
};


template<typename T>
class SegmentedDeque : public Deque<T> {
private:
    SegmentedSequence<T>* segments;
    int segmentSize;
    int count;

    struct {
        int seg;
        int index;
    } front, back;


    MutableArraySequence<T>* CreateFilledSegment() {
        auto* seg = new MutableArraySequence<T>();
        for (int i = 0; i < segmentSize; ++i) {
            seg->Append(T());
        }
        return seg;
    }

    void Resize() {
        int oldSize = segments->GetSegmentsLength();
        int newSize = oldSize * 2;
        int offset = (newSize - oldSize) / 2;

        auto* newSegments = new MutableSegmentedSequence<T, MutableArraySequence>(segmentSize);
        for (int i = 0; i < newSize; ++i) {
            newSegments->GetSegments().Append(CreateFilledSegment());
        }

        for (int i = 0; i < oldSize; ++i) {
            MutableArraySequence<T>* oldSeg = static_cast<MutableArraySequence<T>*>(segments->GetSegment(i));
            MutableArraySequence<T>* newSeg = static_cast<MutableArraySequence<T>*>(newSegments->GetSegment(offset + i));
            
            for (int j = 0; j < oldSeg->GetLength(); ++j) {
                (*newSeg)[j] = oldSeg->Get(j);
            }
        }

        front.seg += offset;
        back.seg += offset;

        delete segments;
        segments = newSegments;
    }

public:
    explicit SegmentedDeque(int segSize = 2)
        : segments(new MutableSegmentedSequence<T, MutableArraySequence>(segSize)),
          segmentSize(segSize),
          count(0),
          front{0, 0},
          back{0, 0} {
        segments->GetSegments().Append(CreateFilledSegment());
        segments->GetSegments().Append(CreateFilledSegment());
    }

    ~SegmentedDeque() override {
        delete segments;
    }

    void PushBack(const T& item) override {
        if (count == 0) {
            front.seg = back.seg = 0;
            front.index = back.index = 0;
        }
        if (back.index == segmentSize) {
            back.seg++;
            back.index = 0;
            if (back.seg >= segments->GetSegmentsLength()) {
                Resize();
            }
        }
        (*segments->GetSegment(back.seg))[back.index] = item;
        back.index++;
        count++;
    }

    void PushFront(const T& item) override {
        if (count == 0) {
            front.seg = back.seg = 0;
            front.index = back.index = 0;
        }
        if (front.index == 0) {
            front.seg--;
            front.index = segmentSize;
            if (front.seg < 0) {
                Resize();
            }
        }
        front.index--;
        (*segments->GetSegment(front.seg))[front.index] = item;
        count++;
    }

    virtual T PopBack() override {
        if (count == 0) throw std::out_of_range("Deque is empty");
        
        back.index--;
        if (back.index < 0) {
            back.seg--;
            back.index = segmentSize - 1;
            if (back.seg < 0) {
                throw std::out_of_range("Invalid segment after PopBack");
            }
        }
        T item = segments->GetSegment(back.seg)->Get(back.index);
        (*segments->GetSegment(back.seg))[back.index] = T();
        count--;
        return item;
    }

    virtual T PopFront() override {
        if (count == 0) throw std::out_of_range("Deque is empty");
        
        T item = segments->GetSegment(front.seg)->Get(front.index);
        (*segments->GetSegment(front.seg))[front.index] = T();
        front.index++;
        if (front.index == segmentSize) {
            front.seg++;
            front.index = 0;
            if (front.seg >= segments->GetSegmentsLength()) {
                front.seg--;
                front.index = segmentSize - 1;
            }
        }
        count--;
        return item;
    }

    virtual const T& Front() const override {
        if (count == 0) throw std::out_of_range("Deque is empty");
        return segments->GetSegment(front.seg)->Get(front.index);
    }
    
    virtual const T& Back() const override {
        if (count == 0) throw std::out_of_range("Deque is empty");
        int idx = back.index - 1;
        int seg = back.seg;
        if (idx < 0) {
            seg--;
            idx = segmentSize - 1;
        }
        return segments->GetSegment(seg)->Get(idx);
    }

    virtual int Size() const override {
        return count;
    }

    virtual bool isEmpty() const override {
        return (count == 0);
    }

    virtual Deque<T>* cloneEmpty() const override {
        return new SegmentedDeque<T>();
    }
};


template <typename T>
class Stack {
private:
    Deque<T>* deque;

    Stack<T>* fromDeque(const Deque<T>* deq) const {
        Stack<T>* stack = new Stack<T>();
        Deque<T>* temp = deq->clone();

        while (!temp->isEmpty()) {
            stack->Push(temp->PopFront());
        }

        delete temp;
        return stack;
    }

public:
    Stack() : deque(new SequenceDeque<T>()) {}

    ~Stack() {
        delete deque;
    }

    void Push(const T& item) {
        deque->PushBack(item);
    }

    T Pop() {
        return deque->PopBack();
    }

    const T& Top() const {
        return deque->Back();
    }

    int Size() const {
        return deque->Size();
    }

    bool isEmpty() const {
        return deque->isEmpty();
    }

    Stack<T>* where(const std::function<bool(const T&)>& predicate) const {
        Deque<T>* filteredDeque = deque->where(predicate);
        Stack<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    Stack<T>* map(const std::function<T(const T&)>& func) const {
        Deque<T>* filteredDeque = deque->map(func);
        Stack<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    T reduce(const std::function<T(const T&, const T&)>& predicate) const {
        return deque->reduce(predicate);
    }

    Stack<T>* concat(const Stack<T>* other) const {
        Deque<T>* filteredDeque = deque->concat(other->deque);
        Stack<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    Stack<T>* subSequence(int start, int end) const {
        Deque<T>* filteredDeque = deque->where(start, end);
        Stack<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    Stack<T>* clone() const {
        Stack<T>* copy = new Stack<T>();
        Stack<T>* temp = new Stack<T>();
        
        Stack<T>* source = const_cast<Stack<T>*>(this);
        while (!source->isEmpty()) {
            T item = source->Pop();
            temp->Push(item);
        }
    
        while (!temp->isEmpty()) {
            T item = temp->Pop();
            source->Push(item);
            copy->Push(item);
        }

        delete temp;
        return copy;
    }
};


template <typename T>
class Queue {
private:
    Deque<T>* deque;

    Queue<T>* fromDeque(const Deque<T>* deq) const {
        Queue<T>* queue = new Queue<T>();
        Deque<T>* temp = deq->clone();

        while (!temp->isEmpty()) {
            queue->Push(temp->PopFront());
        }

        delete temp;
        return queue;
    }

public:
    Queue() : deque(new SequenceDeque<T>()) {}

    ~Queue() {
        delete deque;
    }

    void Push(const T& item) {
        deque->PushBack(item);
    }

    T Pop() {
        return deque->PopFront();
    }

    const T& Front() const {
        return deque->Front();
    }

    const T& Back() const {
        return deque->Back();
    }

    int Size() const {
        return deque->Size();
    }

    bool isEmpty() const {
        return deque->isEmpty();
    }

    Queue<T>* where(const std::function<bool(const T&)>& predicate) const {
        Deque<T>* filteredDeque = deque->where(predicate);
        Queue<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    Queue<T>* map(const std::function<T(const T&)>& func) const {
        Deque<T>* filteredDeque = deque->map(func);
        Queue<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    T reduce(const std::function<T(const T&, const T&)>& predicate) const {
        return deque->reduce(predicate);
    }

    Queue<T>* concat(const Queue<T>* other) const {
        Deque<T>* filteredDeque = deque->concat(other->deque);
        Queue<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }

    Queue<T>* subSequence(int start, int end) const {
        Deque<T>* filteredDeque = deque->where(start, end);
        Queue<T>* result = fromDeque(filteredDeque);
        delete filteredDeque;
        return result;
    }
};