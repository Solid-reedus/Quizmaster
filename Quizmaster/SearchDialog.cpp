#include "SearchDialog.h"


SearchDialog::SearchDialog()
{
	rect = {};
	color = {};
	renderer = nullptr;
	searchDialogTexture = nullptr;
	elmSize = 0;
	relxPos = 0;
}

SearchDialog::SearchDialog(int m_xPos, int m_yPos, int m_width, int m_height, int m_elementHeight,
	SDL_Color m_color, SDL_Renderer* m_renderer)
{
	rect = { m_xPos, m_yPos, m_width, m_height };
	color = m_color;
	renderer = m_renderer;
	elmSize = m_elementHeight;
	relxPos = 0;
	searchDialogTexture = nullptr;

	SDL_Surface* surf = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	Uint32 clr = SDL_MapRGB(surf->format, color.r, color.b, color.g);
	SDL_FillRect(surf, NULL, clr);
	searchDialogTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

SearchDialog::~SearchDialog()
{
	//SDL_DestroyTexture(searchDialogTexture);
}

void SearchDialog::AddItem(ISearchDialogable* m_item)
{
	int y = (elements.size() * (elmSize + 5)) + rect.y;
	y += 5;
	m_item->SetNewRect(rect.x + 5, y, rect.w - 10, elmSize);
	// update height to current height
	m_item->UpdateHeight(0);
	elements.push_back(m_item);
}

void SearchDialog::Render()
{
	SDL_RenderCopy(renderer, searchDialogTexture, NULL, &rect);

	if (elements.size() < 1)
	{
		return;
	}

	int s = relxPos / elmSize;
	if (s > 0)
	{
		s--;
	}
	int m = (relxPos + rect.h) / elmSize + 1;


	SDL_RenderSetClipRect(renderer, &rect);

	for (size_t i = s; i < m; i++)
	{
		if (i > elements.size() - 1)
		{
			break;
		}
		elements[i]->Render();
	}

	SDL_RenderSetClipRect(renderer, nullptr);
}

void SearchDialog::OnScroll(int* m_x, int* m_y, int* m_a)
{
	printf("check scroll \n");
	if (*m_x > rect.x && *m_x < rect.x + rect.w &&
		*m_y > rect.y && *m_y < rect.y + rect.h)
	{
		// +*m_a
		if (*m_a > 0)
		{
			// if relative + increase amount is bigger than the size of all elements * a element size + 5px margin
			// - height of the SearchDialog and subtracting the subtracting with a subtracting so it adds 
			// a extra 1px extra lenght to the max relxPos
			if ((relxPos + *m_a) > ((elements.size() * (elmSize + 5)) - (rect.h - (elements.size() + *m_a))))
			{
				return;
			}
		}
		// -*m_a
		else
		{
			if ((relxPos - *m_a) < 1)
			{
				return;
			}
		}

		relxPos += *m_a;
		printf("scroll \n");
		for (ISearchDialogable* e : elements)
		{
			e->UpdateHeight(*m_a);
		}
	}
}

std::vector<ISearchDialogable*>* SearchDialog::GetElements()
{
	return &elements;
}

void SearchDialog::ResetRelXPos()
{
	relxPos = 0;
}

void SearchDialog::FreeItems()
{

	for (auto it = elements.begin(); it != elements.end(); ) 
	{
		delete *it;  // Deletes the element
		it = elements.erase(it);  // Removes the element from the vector and returns the iterator to the next element
	}

}