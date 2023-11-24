#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream>

namespace BCPP_Package_VectorTable
{
    template <typename columnsTypeStruct>
    class VectorTableInterface
    {
    public:
        virtual void pushRow(columnsTypeStruct row) = 0;
        virtual columnsTypeStruct &returnRow(int indexKey) = 0;
        virtual columnsTypeStruct &getTail() = 0;
        virtual columnsTypeStruct &getHead() = 0;
        virtual void updateRow(int indexKey, columnsTypeStruct row) = 0;
        virtual void deleteRow(int indexKey) = 0;
    };

    template <typename columnsTypeStruct>
    class VectorTable : public VectorTableInterface<columnsTypeStruct>
    {
    private:
        std::vector<columnsTypeStruct> vectorTable;

    public:
        VectorTable(){};
        void pushRow(columnsTypeStruct row)
        {
            vectorTable.push_back(row);
        }

        columnsTypeStruct &returnRow(int indexKey)
        {
            try
            {
                return vectorTable.at(indexKey);
            }
            catch (const std::out_of_range &e)
            {
                return vectorTable.back();
            }
        }

        columnsTypeStruct &getTail()
        {
            return vectorTable.back();
        }

        columnsTypeStruct &getHead()
        {
            return vectorTable[0];
        }

        int returnSize()
        {
            return vectorTable.size();
        }

        bool isEmpty()
        {
            return vectorTable.empty();
        }

        void updateRow(int indexKey, columnsTypeStruct row)
        {
            indexKey > vectorTable.size() ? indexKey = vectorTable.size() - 1 : indexKey = indexKey;
            vectorTable.at(indexKey) = row;
        }

        void deleteRow(int indexKey)
        {
            if (indexKey >= 0 && indexKey < vectorTable.size())
            {
                vectorTable.erase(vectorTable.begin() + indexKey);
            }
        }

        const std::vector<columnsTypeStruct> &returnAllRows()
        {
            return vectorTable;
        }

        template <typename MemberType, typename MemberSelector>
        MemberType sumMember(MemberSelector selector) const
        {
            MemberType total = MemberType{};
            if (vectorTable.size() > 0)
            {
                for (const columnsTypeStruct &row : vectorTable)
                {
                    total += selector(row);
                }
            }
            else
            {
                total = 0;
            }
            return total;
        }

        void sortTableByMember(std::function<bool(const columnsTypeStruct &, const columnsTypeStruct &)> comparator)
        {
            std::sort(vectorTable.begin(), vectorTable.end(), comparator);
        }

        template <typename MemberSelector>
        int findIndexByCustomKey(int keyValue, MemberSelector selector)
        {
            for (int i = 0; i < vectorTable.size(); ++i)
            {
                if (selector(vectorTable[i]) == keyValue)
                {
                    return i;
                }
            }
            return -1;
        }

        std::vector<columnsTypeStruct> getRangeByIndex(int startIndex, int offset)
        {
            std::vector<columnsTypeStruct> range;

            if (startIndex < 0 || startIndex >= vectorTable.size() || offset <= 0)
            {
                return range;
            }

            int endIndex = std::min(startIndex + offset, static_cast<int>(vectorTable.size()));

            range.reserve(endIndex - startIndex);

            for (int i = startIndex; i < endIndex; ++i)
            {
                range.push_back(vectorTable[i]);
            }

            return range;
        }

        template <typename MemberSelector>
        bool swapRowsByPrimaryKey(int primaryKey1, int primaryKey2, MemberSelector serialNumberSelector)
        {
            int index1 = findIndexByCustomKey(primaryKey1, serialNumberSelector);
            int index2 = findIndexByCustomKey(primaryKey2, serialNumberSelector);

            if (index1 != -1 && index2 != -1)
            {
                std::swap(vectorTable[index1], vectorTable[index2]);
                return true;
            }
            else
            {
                return false;
            }
        }

        template <typename MemberSelector>
        typename MemberSelector::result_type getLastPrimaryKeyValue(MemberSelector selector) const
        {
            if (vectorTable.empty())
            {
                return MemberSelector::result_type();
            }

            auto comparator = [&selector](const columnsTypeStruct &a, const columnsTypeStruct &b)
            {
                return selector(a) < selector(b);
            };

            std::sort(vectorTable.begin(), vectorTable.end(), comparator);

            // The last element will have the maximum primary key value
            return selector(vectorTable.back());
        }

        int rowCount() const
        {
            return vectorTable.size();
        }
    };
}