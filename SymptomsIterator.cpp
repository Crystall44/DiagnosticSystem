#include "SymptomsIterator.h"

SymptomsIterator::SymptomsIterator(SymptomComponent* root) : currentIndex(0) {
    if (root) {
        flatSymptoms = root->getAllSymptoms();
    }
}

bool SymptomsIterator::hasNext() const {
    return currentIndex < flatSymptoms.size();
}

std::string SymptomsIterator::next() {
    if (hasNext()) {
        return flatSymptoms[currentIndex++];
    }
    return "";
}

void SymptomsIterator::reset() {
    currentIndex = 0;
}

size_t SymptomsIterator::getSize() const {
    return flatSymptoms.size();
}