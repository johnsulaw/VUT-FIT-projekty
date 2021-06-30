<?php

namespace App\Modules\CoreModule\Presenters;
use Contributte\FormsBootstrap\BootstrapForm;
use Contributte\FormsBootstrap\BootstrapRenderer;
use Contributte\FormsBootstrap\Enums\RendererOptions;
use Contributte\FormsBootstrap\Enums\RenderMode;
use Nette;
use Tracy\Debugger;

final class HomepagePresenter extends Nette\Application\UI\Presenter
{

    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
	
	public function renderDefault(): void
	{
        $this->template->hotels = $this->database->table('hotel');

        $this->template->data = $this->database->table('hotel');
        $this->template->cities = array();
        foreach ($this->template->data as $d) {
            array_push($this->template->cities, $d->city);
            array_push($this->template->cities, $d->name);
        }

        $this->template->dateMin = date("m/d/Y");
        $this->template->dateMax = date("m/d/Y", mktime(0,0,0,date("m") + 1));
	}

    protected function createComponentSearchForm()
    {
        $form = new BootstrapForm;
       


        $form->addText('city')
            ->setAutocomplete(TRUE)
            ->setHtmlAttribute('placeholder', 'Give me room in...')
            ->setHtmlAttribute('class', 'typeahead tt-query')
            ->setHtmlAttribute('style','color: white; border-bottom: 1px solid #4dd0e1; box-shadow: 0 1px 0 0 #4dd0e1;')
            ->setHtmlAttribute('spellcheck', 'false')
            ->setHtmlAttribute('type', 'text');

        $form->addSubmit('submit_val', 'Search')
            ->setHtmlAttribute('class','search-city btn btn-success');

        $form->onSuccess[] = [$this, 'formSucceeded'];

        return $form;
    }



    public function formSucceeded(BootstrapForm $form, $data)
    {
        $hotels = $this->database->table('hotel');
        
        foreach ($hotels as $hotel) {
            if (!strcasecmp($data->city, $hotel->name)){
                $this->redirect('Room:', $hotel->id);
            }
        }
        $this->redirect('Homepage:hotels', $data->city);
    }

    public function renderHotels($data): void
    {
        $this->template->city = $data;

        $this->template->hotels = $this->database->table('hotel');

        $this->template->hotelsOut = array();
        $this->template->hotelCnt = 0;
        $this->template->galleries = array();
        
        // parsovanie hotela na ziskanie nazvov hotelov a galerii
        foreach ($this->template->hotels as $hotel) {
            if (!strcasecmp($this->template->city, $hotel->city)){
                array_push($this->template->hotelsOut, $hotel);
                $this->template->galleries[$hotel->name] = $hotel->gallery;
                $this->template->hotelCnt++;
            }
        }
        
        $this->template->images = array();
        foreach ($this->template->galleries as $gallery => $galleryName)
        {
            $directory = "../www/img/galleries/".$galleryName;
            $scanned_directory = array_diff(scandir($directory), array('..', '.'));
            $this->template->images[$galleryName] = $scanned_directory;
        }
    }
}
