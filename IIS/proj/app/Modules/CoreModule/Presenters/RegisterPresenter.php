<?php

declare(strict_types=1);

namespace App\Modules\CoreModule\Presenters;
use Nette;
use Nette\Application\UI\Form;


final class RegisterPresenter extends Nette\Application\UI\Presenter
{
    protected function createComponentRegistrationForm(): Form
	{
		
        $form = new Form;

        $form->onSuccess[] = [$this, 'formSucceeded'];

        $form->addText('name', 'Email')
            ->setRequired('Enter your name');

        $form->addPassword('password', 'Password');

        $form->addSubmit('submit', 'Sign up');

        $renderer = $form->getRenderer();
        $renderer->wrappers['controls']['container'] = null;
        $renderer->wrappers['pair']['container'] = 'div class="form-group row"';
        $renderer->wrappers['pair']['.error'] = 'has-danger';
        $renderer->wrappers['control']['container'] = 'div class=col-sm-9';
        $renderer->wrappers['label']['container'] = 'div class="col-sm-3 col-form-label"';
        $renderer->wrappers['control']['description'] = 'span class=form-text';
        $renderer->wrappers['control']['errorcontainer'] = 'span class=form-control-feedback';
        $renderer->wrappers['control']['.error'] = 'is-invalid';

	    foreach ($form->getControls() as $control) {
            $type = $control->getOption('type');
            if ($type === 'button') {
                $control->getControlPrototype()->addClass(empty($usedPrimary) ? 'btn btn-primary' : 'btn btn-secondary');
                $usedPrimary = true;

            } elseif (in_array($type, ['text', 'textarea', 'select'], true)) {
                $control->getControlPrototype()->addClass('form-control');

            }
        }

		return $form;
	}

	public function formSucceeded(Form $form, $data): void
	{
		$this->flashMessage('You have been registered');
		$this->redirect('Homepage:');
    }

}